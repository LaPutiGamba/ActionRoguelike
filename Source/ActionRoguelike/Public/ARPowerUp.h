#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARInteractInterface.h"
#include "ARPowerUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARPowerUp : public AActor, public IARInteractInterface
{
	GENERATED_BODY()


public:	
	AARPowerUp();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) const override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UPROPERTY(EditAnywhere, Category = "PowerUp")
	float RespawnTime;

	FTimerHandle TimerHandleRespawn;

	void ShowPowerUp();
	void HideAndCooldownPowerUp();

	void SetPowerUpState(bool bNewIsActive);

	UFUNCTION()
	void OnRep_IsActive();
};
