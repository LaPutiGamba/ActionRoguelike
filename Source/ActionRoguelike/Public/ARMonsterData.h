#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ARMonsterData.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<class UARAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId("Monsters", GetFName()); }
};
