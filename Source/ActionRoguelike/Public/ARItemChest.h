#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARInteractInterface.h"
#include "ARItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARItemChest : public AActor, public IARInteractInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void OnActorLoaded_Implementation() override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> LidMeshComp;

	UPROPERTY(ReplicatedUsing = "OnRep_LipOpened", BlueprintReadOnly, SaveGame)
	bool bIsOpened;

	UFUNCTION()
	void OnRep_LipOpened();

public:	
	AARItemChest();

	UPROPERTY(EditAnywhere)
	float OpenAnglePitch;
};
