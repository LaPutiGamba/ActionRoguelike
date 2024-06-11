#include "ARItemChest.h"
#include "Net/UnrealNetwork.h"

AARItemChest::AARItemChest()
{
	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	RootComponent = BaseMeshComp;

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComp->SetupAttachment(BaseMeshComp);

	OpenAnglePitch = 110.f;

	bIsOpened = false;

	bReplicates = true;
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsOpened = !bIsOpened;

	OnRep_LipOpened();
}

void AARItemChest::OnActorLoaded_Implementation()
{
	OnRep_LipOpened();
}

void AARItemChest::OnRep_LipOpened()
{
	float CurrentPitch = bIsOpened ? OpenAnglePitch : 0.f;
	LidMeshComp->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

void AARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItemChest, bIsOpened);
}