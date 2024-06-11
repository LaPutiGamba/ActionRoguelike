#include "ARPowerUp.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AARPowerUp::AARPowerUp()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	SphereComp->SetSphereRadius(90.f);
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.f;

	bReplicates = true;
}

void AARPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
}

FText AARPowerUp::GetInteractText_Implementation(APawn* InstigatorPawn) const
{
	return FText::GetEmpty();
}

void AARPowerUp::ShowPowerUp()
{
	SetPowerUpState(true);

	RootComponent->SetVisibility(true, true);
}

void AARPowerUp::HideAndCooldownPowerUp()
{
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandleRespawn, this, &AARPowerUp::ShowPowerUp, RespawnTime);
}

void AARPowerUp::SetPowerUpState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void AARPowerUp::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void AARPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPowerUp, bIsActive);
}