#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AARMagicProjectile();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> ParticleComp;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
