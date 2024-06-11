#pragma once

#include "CoreMinimal.h"
#include "ARPowerUp.h"
#include "ARHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARHealthPotion : public AARPowerUp
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) const override;

public:
	AARHealthPotion();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HealthPotion")
	float HealthToAdd;

	UPROPERTY(EditDefaultsOnly, Category = "HealthPotion")
	int32 CreditCost;
};
