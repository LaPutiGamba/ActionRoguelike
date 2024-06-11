#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacterAI.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARCharacterAI : public ACharacter
{
	GENERATED_BODY()

public:
	AARCharacterAI();

protected:
	TObjectPtr<class UARWorldUserWidget> HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UARAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UARActionComponent> ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta);
};
