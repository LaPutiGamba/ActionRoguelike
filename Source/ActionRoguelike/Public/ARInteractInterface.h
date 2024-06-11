#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARInteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UARInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class ACTIONROGUELIKE_API IARInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetInteractText(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(APawn* InstigatorPawn);
};
