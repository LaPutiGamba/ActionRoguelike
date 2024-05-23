#include "ARExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"

AARExplosiveBarrel::AARExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 700.f;
	RadialForceComp->ImpulseStrength = 2000.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
}

void AARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &AARExplosiveBarrel::OnHit);
}

void AARExplosiveBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}