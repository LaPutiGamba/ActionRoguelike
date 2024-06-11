#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARInteractionComponent.generated.h"

class UARWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();

protected:
	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UARWorldUserWidget> DefaultWidgetInstance;

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();
};
