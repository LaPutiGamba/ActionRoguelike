#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "GameplayTagContainer.h"
#include "ARProjectileMagic.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARProjectileMagic : public AARProjectileBase
{
	GENERATED_BODY()

public:
	AARProjectileMagic();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	FTimerHandle TimerHandleLifetime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundWave> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	TSubclassOf<class UARActionEffect> BurningActionClass;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
