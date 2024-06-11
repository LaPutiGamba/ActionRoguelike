#pragma once

#include "CoreMinimal.h"
#include "ARActionEffect.h"
#include "ARActionEffect_Thorns.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARActionEffect_Thorns : public UARActionEffect
{
	GENERATED_BODY()

public:
	UARActionEffect_Thorns();

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, class UARAttributeComponent* OwningComp, float NewHealth, float Delta);
};
