#include "ARTargetDummy.h"
#include "ARAttributeComponent.h"

AARTargetDummy::AARTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComp"));
}

void AARTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARTargetDummy::OnHealthChanged);
}

void AARTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}