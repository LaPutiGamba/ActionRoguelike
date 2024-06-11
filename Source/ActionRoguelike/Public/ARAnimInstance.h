#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARAnimInstance.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UARActionComponent> ActionComp;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
