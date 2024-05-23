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
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComp;

	// Input Mappings
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Mappings")
	TObjectPtr<UInputMappingContext> DefaultMapping;

	// Input
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "ARCharacter|Input Actions")
	TObjectPtr<UInputAction> PrimaryAttackAction;

	// Projectile
	UPROPERTY(EditAnywhere, Category = "ARCharacter|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void PrimaryAttack();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
