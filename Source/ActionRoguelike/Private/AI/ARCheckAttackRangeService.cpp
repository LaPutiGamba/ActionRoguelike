#include "AI/ARCheckAttackRangeService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARCheckAttackRangeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp)) {
		if (AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"))) {
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController)) {
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn)) {
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.f;
					bool bHasLOS = false;
					if (bWithinRange)
						bHasLOS = MyController->LineOfSightTo(TargetActor);

					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
