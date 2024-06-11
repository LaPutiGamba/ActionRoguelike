#include "ARProjectileDash.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AARProjectileDash::AARProjectileDash()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	ProjectileMovementComp->InitialSpeed = 6000.f;
}

void AARProjectileDash::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &AARProjectileDash::Explode, DetonateDelay);
}

void AARProjectileDash::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	ProjectileMovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TeleportTimerHandle;
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &AARProjectileDash::TeleportInstigator, TeleportDelay);
}

void AARProjectileDash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport)) {
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	}
}