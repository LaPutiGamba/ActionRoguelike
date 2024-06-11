#pragma once

#include "CoreMinimal.h"
#include "ARPowerUp.h"
#include "ARPowerUpAction.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARPowerUpAction : public AARPowerUp
{
	GENERATED_BODY()

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditAnywhere, Category = "PowerUp")
	TSubclassOf<class UARAction> ActionToGrant;
};
