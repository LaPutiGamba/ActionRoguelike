#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "ARProjectileDash.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARProjectileDash : public AARProjectileBase
{
	GENERATED_BODY()

public:
	AARProjectileDash();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle ExplodeTimerHandle;

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;
	
	void TeleportInstigator();
};
