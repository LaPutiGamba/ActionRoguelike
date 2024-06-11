#include "ARHealthPotion.h"
#include "ARAttributeComponent.h"
#include "ARPlayerState.h"

AARHealthPotion::AARHealthPotion()
{
	HealthToAdd = 50.f;
	CreditCost = 50;
}

void AARHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn) 
		return;

	if (UARAttributeComponent* AttributeComp = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()))) {
		if (AttributeComp->GetHealth() < AttributeComp->GetMaxHealth()) {
			if (AARPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AARPlayerState>()) {
				if (PlayerState->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, HealthToAdd))
					HideAndCooldownPowerUp();
			}
		}
	}
}

FText AARHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn) const
{
	if (UARAttributeComponent* AttributeComp = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()))) {
		if (AttributeComp->GetHealth() == AttributeComp->GetMaxHealth()) {
			return NSLOCTEXT("InteractableActors", "HealthPotion_FullHealthWarning", "Already at full health.");
		}
	}
	return FText::Format(NSLOCTEXT("InteractableActors", "HealthPotion_InteractText", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}
