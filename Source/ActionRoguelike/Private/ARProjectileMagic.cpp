#include "ARProjectileMagic.h"
#include "ARActionComponent.h"
#include "ARGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ARActionEffect.h"

AARProjectileMagic::AARProjectileMagic()
{
	SphereComp->SetSphereRadius(20.0f);

	InitialLifeSpan = 10.0f;

	DamageAmount = 20.0f;
}

void AARProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARProjectileMagic::OnActorOverlap);
}

void AARProjectileMagic::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComp->GetSound())
		AudioComp->Play();

	GetWorldTimerManager().SetTimer(TimerHandleLifetime, [this](){
		if (AudioComp->IsPlaying())
			AudioComp->Stop();
		Explode();
	}, 2.0f, false);
}

void AARProjectileMagic::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorldTimerManager().ClearTimer(TimerHandleLifetime);
	if (AudioComp->IsPlaying())
		AudioComp->Stop();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());

	if (OtherActor && OtherActor != GetInstigator()) {
		UARActionComponent* ActionComp = Cast<UARActionComponent>(OtherActor->GetComponentByClass(UARActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag)) {
			ProjectileMovementComp->Velocity = -ProjectileMovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}


		if (UARGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult)) {
			Explode();

			if (ActionComp && HasAuthority()) {
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}
