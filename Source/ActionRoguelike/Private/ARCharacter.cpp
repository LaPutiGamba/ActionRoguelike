#include "ARCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera settings
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(DefaultMapping, 0);
	}
}

void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Movement
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AARCharacter::Move);
		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARCharacter::Look);

		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

		// Primary Attack
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &AARCharacter::PrimaryAttack);
	}
}

void AARCharacter::Move(const FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();

	FRotator ControlRotation = GetControlRotation();
	FVector ForwardVector = FRotator(0.f, ControlRotation.Yaw, 0.f).Vector();
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.Y);
	AddMovementInput(RightVector, MovementVector.X);
}

void AARCharacter::Look(const FInputActionValue& Value)
{
	const auto LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(-LookVector.Y);
}

void AARCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("HandSocket");

	FTransform SpawnTransform = FTransform(GetActorRotation(), HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}
