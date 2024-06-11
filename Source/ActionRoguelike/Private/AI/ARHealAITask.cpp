#include "AI/ARHealAITask.h"
#include "AIController.h"
#include "ARAttributeComponent.h"

EBTNodeResult::Type UARHealAITask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController)) {
		APawn* MyPawn = MyController->GetPawn();
		if (MyPawn == nullptr)
			return EBTNodeResult::Failed;

		bool HealApplied = false;
		if (auto AttributeComp = Cast<UARAttributeComponent>(MyPawn->GetComponentByClass(UARAttributeComponent::StaticClass()))) {
			HealApplied = AttributeComp->ApplyHealthChange(nullptr, AttributeComp->GetMaxHealth());
		}

		return HealApplied ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
