#include "ARCharacter.h"
#include "ARAttributeComponent.h"
#include "ARActionComponent.h"
#include "ARInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../SharedGameplayTags.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera settings
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// Interaction component
	InteractionComp = CreateDefaultSubobject<UARInteractionComponent>("InteractionComp");

	// Attribute component
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("AttributeComp");

	// Action component
	ActionComp = CreateDefaultSubobject<UARActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARCharacter::OnHealthChanged);
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(DefaultMapping, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Movement
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &AARCharacter::Move);
		// Sprint
		EnhancedInputComponent->BindAction(ActionSprint, ETriggerEvent::Triggered, this, &AARCharacter::Sprint);
		// Look
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &AARCharacter::Look);
		// Jump
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);

		// Primary Interact
		EnhancedInputComponent->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &AARCharacter::PrimaryInteract);

		// Primary Attack
		EnhancedInputComponent->BindAction(ActionPrimaryAttack, ETriggerEvent::Triggered, this, &AARCharacter::PrimaryAttack);

		// Secondary Attack
		EnhancedInputComponent->BindAction(ActionSecondaryAttack, ETriggerEvent::Triggered, this, &AARCharacter::BlackHoleAttack);

		// Dash
		EnhancedInputComponent->BindAction(ActionDash, ETriggerEvent::Triggered, this, &AARCharacter::Dash);
	}
}

void AARCharacter::Move(const FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();

	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	FVector ForwardVector = FRotator(ControlRotation).Vector();
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.Y);
	AddMovementInput(RightVector, MovementVector.X);
}

void AARCharacter::Sprint(const FInputActionValue& Value)
{
	const auto SprintValue = Value.Get<float>();

    if (SprintValue > 0.f) {
        ActionComp->StartActionByTag(this, SharedGameplayTags::Action_Sprint);
    } else {
        ActionComp->StopActionByTag(this, SharedGameplayTags::Action_Sprint);
    }
}

void AARCharacter::Look(const FInputActionValue& Value)
{
	const auto LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(-LookVector.Y);
}

void AARCharacter::PrimaryInteract()
{
	if (InteractionComp)
		InteractionComp->PrimaryInteract();
}

void AARCharacter::PrimaryAttack()
{
	ActionComp->StartActionByTag(this, SharedGameplayTags::Action_PrimaryAttack);
}

void AARCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByTag(this, SharedGameplayTags::Action_Blackhole);
}

void AARCharacter::Dash()
{
	ActionComp->StartActionByTag(this, SharedGameplayTags::Action_Dash);
}

void AARCharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f) {
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);

		if (NewHealth <= 0.f) {
			DisableInput(GetController<APlayerController>());

			SetLifeSpan(5.f);
		}
	}
}

void AARCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}