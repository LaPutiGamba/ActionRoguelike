#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARCheckAttackRangeService.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARCheckAttackRangeService : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
