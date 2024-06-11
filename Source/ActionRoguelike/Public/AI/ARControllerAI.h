#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARControllerAI.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARControllerAI : public AAIController
{
	GENERATED_BODY()

public:
	AARControllerAI();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	virtual void BeginPlay() override;
};
