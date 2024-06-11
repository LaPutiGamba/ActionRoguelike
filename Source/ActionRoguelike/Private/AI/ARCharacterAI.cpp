#include "AI/ARCharacterAI.h"
#include "AIController.h"
#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "ARWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AARCharacterAI::AARCharacterAI()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UARActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetGenerateOverlapEvents(true);

	TargetActorKey = "TargetActor";
}

void AARCharacterAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AARCharacterAI::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARCharacterAI::OnHealthChanged);
}

void AARCharacterAI::SetTargetActor(AActor* NewTarget)
{
	if (auto AIController = Cast<AAIController>(GetController())) {
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

AActor* AARCharacterAI::GetTargetActor() const
{
	if (AAIController* CharacterAI = Cast<AAIController>(GetController()))
		return Cast<AActor>(CharacterAI->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));

	return nullptr;
}

void AARCharacterAI::OnPawnSeen(APawn* SeenPawn)
{
	if (GetTargetActor() != SeenPawn) {
		SetTargetActor(SeenPawn);

		MulticastPawnSeen();
	}
}

void AARCharacterAI::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f) {
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		if (HealthBarWidget == nullptr && HealthBarWidgetClass) {
			HealthBarWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (HealthBarWidget) {
				HealthBarWidget->AttachedActor = this;
				HealthBarWidget->AddToViewport();
			}
		}

		if (InstigatorActor != this)
			SetTargetActor(InstigatorActor);

		if (NewHealth <= 0.f) {
			if (auto AIController = Cast<AAIController>(GetController())) {
				AIController->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.f);
		}
	}
}

void AARCharacterAI::MulticastPawnSeen_Implementation()
{
	if (UARWorldUserWidget* NewWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), SpottedWidgetClass)) {
		NewWidget->AttachedActor = this;
		NewWidget->AddToViewport(10);
	}
}