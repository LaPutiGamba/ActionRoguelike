#include "ARActionComponent.h"
#include "ARAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority()) {
		for (TSubclassOf<UARAction> ActionClass : DefaultActions) {
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UARActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UARAction*> ActionsCopy = Actions;
	for (UARAction* Action : ActionsCopy) {
		if (Action && Action->IsRunning()) {
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
	//PrintStringInfinite(DebugMsg);

	/*for (UARAction* Action : Actions) {
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}*/
}

void UARActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARAction> ActionClass)
{
	if (!ensure(ActionClass))
		return;

	if (!GetOwner()->HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UARAction* NewAction = NewObject<UARAction>(GetOwner(), ActionClass);
	if (ensure(NewAction)) {
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {
			NewAction->StartAction(Instigator);
		}
	}
}

void UARActionComponent::RemoveAction(UARAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) 
		return;

	Actions.Remove(ActionToRemove);
}

UARAction* UARActionComponent::GetAction(const TSubclassOf<UARAction>& ActionClass) const
{
	for (auto Action : Actions) {
		if (Action && Action->IsA(ActionClass)) {
			return Action;
		}
	}

	return nullptr;
}

bool UARActionComponent::StartActionByTag(AActor* Instigator, FGameplayTag ActionName)
{
	for (UARAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (!Action->CanStart(Instigator))
				continue;

			if (!GetOwner()->HasAuthority())
				ServerStartAction(Instigator, ActionName);

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UARActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag ActionName)
{
	for (UARAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->IsRunning()) {
				if (!GetOwner()->HasAuthority())
					ServerStopAction(Instigator, ActionName);

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UARActionComponent::ServerStartAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	StartActionByTag(Instigator, ActionName);
}

void UARActionComponent::ServerStopAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	StopActionByTag(Instigator, ActionName);
}

bool UARActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARAction* Action : Actions) {
		if (Action)
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void UARActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARActionComponent, Actions);
}
