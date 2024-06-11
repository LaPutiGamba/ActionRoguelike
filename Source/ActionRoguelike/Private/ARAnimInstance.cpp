#include "ARAnimInstance.h"
#include "ARActionComponent.h"
#include "GameplayTagContainer.h"

void UARAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor()) {
		ActionComp = Cast<UARActionComponent>(OwningActor->GetComponentByClass(UARActionComponent::StaticClass()));
	}
}

void UARAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp) {
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
