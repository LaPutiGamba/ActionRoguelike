#include "ARCreditsPowerUp.h"
#include "ARPlayerState.h"

AARCreditsPowerUp::AARCreditsPowerUp()
{
	CreditsAmount = 100;
}

void AARCreditsPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
		return;

	if (AARPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AARPlayerState>()) {
		PlayerState->AddCredits(CreditsAmount);
		HideAndCooldownPowerUp();
	}
}