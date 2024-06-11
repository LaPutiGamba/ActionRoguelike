#include "AI/ARCheckLowHealthService.h"
#include "AIController.h"
#include "ARAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARCheckLowHealthService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp)) {
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController)) {
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn)) {
				if (UARAttributeComponent* AttributeComp = Cast<UARAttributeComponent>(AIPawn->GetComponentByClass(UARAttributeComponent::StaticClass()))) {
					float CurrentHealth = AttributeComp->GetHealth();
					if (CurrentHealth < LowHealthToCheck)
						BlackBoardComp->SetValueAsBool(RunAndHide.SelectedKeyName, true);
					else
						BlackBoardComp->SetValueAsBool(RunAndHide.SelectedKeyName, false);
				}
			}
		}
	}
}
