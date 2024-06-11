#pragma once

#include "CoreMinimal.h"
#include "ARPowerUp.h"
#include "ARCreditsPowerUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARCreditsPowerUp : public AARPowerUp
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsAmount;

public:
	AARCreditsPowerUp();
};
