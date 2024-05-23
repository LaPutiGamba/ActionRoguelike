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
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	// Input Mappings
	UPROPERTY(EditAnywhere, Category = "Input Mappings")
	UInputMappingContext* DefaultMapping;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputAction* LookAction;

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
