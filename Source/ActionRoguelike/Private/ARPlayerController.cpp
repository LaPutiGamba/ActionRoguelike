#include "ARPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

void AARPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void AARPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}

void AARPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport()) {
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);

	if (PauseMenuInstance) {
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		// Pause Menu
		EnhancedInputComponent->BindAction(ActionPauseGame, ETriggerEvent::Triggered, this, &AARPlayerController::TogglePauseMenu);
	}
}
