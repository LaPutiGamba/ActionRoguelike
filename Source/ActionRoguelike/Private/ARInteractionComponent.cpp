#include "ARInteractionComponent.h"
#include "ARInteractInterface.h"
#include "ARWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.f;
	CollisionChannel = ECC_WorldDynamic;
}

void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn && OwnerPawn->IsLocallyControlled())
		FindBestInteractable();
}

void UARInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UARInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
		return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	IARInteractInterface::Execute_Interact(InFocus, OwnerPawn);
}

void UARInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	FocusedActor = nullptr;

	FVector Start = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector End = Start + (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Vector() * TraceDistance);
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams)) {
		if (AActor* HitActor = Hit.GetActor()) {
			if (HitActor->Implements<UARInteractInterface>()) {
				FocusedActor = HitActor;
			}
		}
	}

	if (FocusedActor) {
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass)) {
			DefaultWidgetInstance = CreateWidget<UARWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		
		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	} else {
		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->RemoveFromParent();
			DefaultWidgetInstance = nullptr;
		}
	}
}
