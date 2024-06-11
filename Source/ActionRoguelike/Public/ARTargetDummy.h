#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARTargetDummy.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	AARTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UARAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta);
};
