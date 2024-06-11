#include "ARAction.h"
#include "ARActionComponent.h"
#include "Net/UnrealNetwork.h"

UARAction::UARAction()
{
	RepData.bIsRunning = false;
	RepData.Instigator = nullptr;
	bAutoStart = true;

	TimeStarted = 0.f;
}

void UARAction::Initialize(UARActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UARAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning)
		return false;

	UARActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
		return false;

	return true;
}

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UARActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantedTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (Comp->GetOwnerRole() == ROLE_Authority)
		TimeStarted = GetWorld()->GetTimeSeconds();

	Comp->OnActionStarted.Broadcast(Comp, this);
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	UARActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = nullptr;

	Comp->OnActionStopped.Broadcast(Comp, this);
}

void UARAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
		StartAction(RepData.Instigator);
	else
		StopAction(RepData.Instigator);
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	return ActionComp;
}

UWorld* UARAction::GetWorld() const
{
	if (AActor* Actor = Cast<AActor>(GetOuter())) {
		return Actor->GetWorld();
	}

	return nullptr;
}

void UARAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAction, RepData);
	DOREPLIFETIME(UARAction, TimeStarted);
	DOREPLIFETIME(UARAction, ActionComp);
}