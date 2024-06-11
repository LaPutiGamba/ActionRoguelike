#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARCheckLowHealthService.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARCheckLowHealthService : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthToCheck;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector RunAndHide;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
