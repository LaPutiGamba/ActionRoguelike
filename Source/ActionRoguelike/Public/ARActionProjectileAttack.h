#pragma once

#include "CoreMinimal.h"
#include "ARAction.h"
#include "ARActionProjectileAttack.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARActionProjectileAttack : public UARAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelayElapsed(ACharacter* InstigatorCharacter);

public:
	UARActionProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;
};
