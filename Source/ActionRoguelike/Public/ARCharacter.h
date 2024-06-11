#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class ACTIONROGUELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARCharacter();

protected:
	// Camera
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Components")
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "ARCharacter|Components")
	TObjectPtr<class UCameraComponent> CameraComp;

	// Interaction component
	UPROPERTY(VisibleAnywhere, Category = "ARCharacter|Components")
	TObjectPtr<class UARInteractionComponent> InteractionComp;

	// Attribute component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ARCharacter|Components")
	TObjectPtr<class UARAttributeComponent> AttributeComp;

	// Action component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ARCharacter|Components")
	TObjectPtr<class UARActionComponent> ActionComp;

	// Input Mappings
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Mappings")
	TObjectPtr<UInputMappingContext> DefaultMapping;

	// Input
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionMove;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionSprint;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionLook;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionJump;

	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionInteract;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionPrimaryAttack;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionSecondaryAttack;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> ActionDash;

	virtual void PostInitializeComponents() override;
	virtual FVector GetPawnViewLocation() const override;

	void Move(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void PrimaryInteract();

	void PrimaryAttack();
	void BlackHoleAttack();
	void Dash();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};
