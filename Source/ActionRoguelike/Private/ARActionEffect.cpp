#include "ARActionEffect.h"
#include "ARActionComponent.h"
#include "GameFramework/GameStateBase.h"

void UARActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
		
	if (Period > 0.f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UARActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) {
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (UARActionComponent* Comp = GetOwningComponent()) {
		Comp->RemoveAction(this);
	}
}

float UARActionEffect::GetTimeRemaining() const
{
	if (AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>()) {
		float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void UARActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
