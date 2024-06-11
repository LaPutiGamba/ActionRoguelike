#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "Net/UnrealNetwork.h"

void AARPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
		return;

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AARPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
		return false;

	if (Credits < Delta)
		return false;

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

void AARPlayerState::SavePlayerState_Implementation(UARSaveGame* SaveObject)
{
	if (SaveObject) {
		SaveObject->Credits = Credits;
	}
}

void AARPlayerState::LoadPlayerState_Implementation(UARSaveGame* SaveObject)
{
	if (SaveObject) {
		AddCredits(SaveObject->Credits);
	}
}

void AARPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPlayerState, Credits);
}