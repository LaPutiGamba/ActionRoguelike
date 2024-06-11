#include "ARActionEffect_Thorns.h"
#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARGameplayFunctionLibrary.h"

UARActionEffect_Thorns::UARActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UARActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (UARAttributeComponent* Attributes = UARAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner()))
		Attributes->OnHealthChanged.AddDynamic(this, &UARActionEffect_Thorns::OnHealthChanged);
}

void UARActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	if (UARAttributeComponent* Attributes = UARAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner()))
		Attributes->OnHealthChanged.RemoveDynamic(this, &UARActionEffect_Thorns::OnHealthChanged);
}

void UARActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor) {
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
			return;

		ReflectedAmount = FMath::Abs(ReflectedAmount);

		UARGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}