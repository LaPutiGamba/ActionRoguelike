#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARExplosiveBarrel.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	AARExplosiveBarrel();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere)
	class URadialForceComponent* RadialForceComp;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};