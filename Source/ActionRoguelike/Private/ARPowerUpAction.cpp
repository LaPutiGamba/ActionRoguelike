#include "ARPowerUpAction.h"
#include "ARActionComponent.h"
#include "ARAction.h"

void AARPowerUpAction::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
		return;

	if (UARActionComponent* ActionComp = Cast<UARActionComponent>(InstigatorPawn->GetComponentByClass(UARActionComponent::StaticClass()))) {
		if (ActionComp->GetAction(ActionToGrant)) {
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerUp();
	}
}
