#include "ARAttributeComponent.h"
#include "ARGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.DamageMultiplier"), true, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

UARAttributeComponent::UARAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;

	Rage = 0.0f;
	RageMax = 100.0f;

	SetIsReplicatedByDefault(true);
}

void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = HealthMax;
}

UARAttributeComponent* UARAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor) {
		return FromActor->FindComponentByClass<UARAttributeComponent>();
	}

	return nullptr;
}

bool UARAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (UARAttributeComponent* AttributeComp = GetAttributes(Actor))
		return AttributeComp->IsAlive();

	return false;
}

bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	if (!GetOwner()->CanBeDamaged() && DeltaHealth < 0.0f)
		return false;

	if (DeltaHealth < 0.0f) {
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		DeltaHealth *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority()) {
		Health = NewHealth;

		if (ActualDelta != 0.f)
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);

		// Died
		if (ActualDelta < 0.0f && Health == 0.0f) {
			if (AARGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AARGameModeBase>()) {
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

bool UARAttributeComponent::ApplyRage(AActor* InstigatorActor, float DeltaRage)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + DeltaRage, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);

	return ActualDelta != 0;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

void UARAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UARAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UARAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAttributeComponent, Health);
	DOREPLIFETIME(UARAttributeComponent, HealthMax);
	DOREPLIFETIME(UARAttributeComponent, Rage);
	DOREPLIFETIME(UARAttributeComponent, RageMax);
}