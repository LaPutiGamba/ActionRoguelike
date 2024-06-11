#include "ARGameModeBase.h"

#include "ARActionComponent.h"
#include "ARPlayerState.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARInteractInterface.h"
#include "ARMonsterData.h"
#include "ARSaveGame.h"
#include "EngineUtils.h"
#include "AI/ARCharacterAI.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable/Disable Bots Spawning via Timer."), ECVF_Cheat);

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 5.0f;
	CreditsPerKill = 50;

	PowerupSpawnQuery = nullptr;

	DesiredPowerUpCount = 10;
	RequiredPowerUpDistance = 2000;

	PlayerStateClass = AARPlayerState::StaticClass();

	SaveSlotName = "SaveGame01";
}

void AARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (!SelectedSaveSlot.IsEmpty()) {
		SaveSlotName = SelectedSaveSlot;
	}

	LoadSaveGame();
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandleSpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (ensure(PowerUpClasses.Num() > 0)) {
		FEnvQueryRequest Request(PowerupSpawnQuery, this);
		Request.Execute(EEnvQueryRunMode::AllMatching, this, &AARGameModeBase::OnPowerupSpawnQueryCompleted);
	}
}

void AARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (AARPlayerState* PlayerState = NewPlayer->GetPlayerState<AARPlayerState>()) {
		if (PlayerState && CurrentSaveGame) {
			PlayerState->LoadPlayerState(CurrentSaveGame);
		}
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AARGameModeBase::KillAll()
{
	for (TActorIterator<AARCharacterAI> It(GetWorld()); It; ++It) {
		AARCharacterAI* Bot = *It;

		UARAttributeComponent* AttributeComp = UARAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
			AttributeComp->Kill(this);
	}
}

void AARGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("Bots spawning is disabled via 'CVarSpawnBots'."));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<AARCharacterAI> It(GetWorld()); It; ++It) {
		AARCharacterAI* Bot = *It;

		UARAttributeComponent* AttributeComp = UARAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
			NrOfAliveBots++;
	}

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());

	if (NrOfAliveBots >= MaxBotCount)
		return;

	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &AARGameModeBase::OnBotSpawnQueryCompleted);
}

void AARGameModeBase::OnBotSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful()) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	if (Locations.IsValidIndex(0) && MonsterTable) {
		UAssetManager& Manager = UAssetManager::Get();

		TArray<FMonsterInfoRow*> Rows;
		MonsterTable->GetAllRows("", Rows);

		int32 RandomRowIndex = FMath::RandRange(0, Rows.Num() - 1);
		FMonsterInfoRow* SelectedRow = Rows[RandomRowIndex];

		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AARGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
		Manager.LoadPrimaryAsset(SelectedRow->MonsterId, TArray<FName>(), Delegate);
	}
}

void AARGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized()) {
		if (UARMonsterData* MonsterData = Cast<UARMonsterData>(AssetManager->GetPrimaryAssetObject(LoadedId))) {
			if (AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator)) {
				if (UARActionComponent* ActionComp = Cast<UARActionComponent>(NewBot->GetComponentByClass(UARActionComponent::StaticClass()))) {
					for (TSubclassOf<UARAction> ActionClass : MonsterData->Actions) {
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}
}

void AARGameModeBase::OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful()) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn power up EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	while (SpawnCounter < DesiredPowerUpCount && Locations.Num() > 0) {
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations) {
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerUpDistance) {
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
			continue;

		int32 RandomClassIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerUpClass = PowerUpClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerUpClass, PickedLocation, FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void AARGameModeBase::RespawnPlayer(AController* Controller)
{
	if (ensure(Controller)) {
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AARGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	if (AARCharacter* Player = Cast<AARCharacter>(VictimActor)) {
		FTimerHandle TimerHandleRespawnDelay;

		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, "RespawnPlayer", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandleRespawnDelay, RespawnDelegate, 2.0f, false);
	}

	if (APawn* KillerPawn = Cast<APawn>(Killer)) {
		if (AARPlayerState* PlayerState = KillerPawn->GetPlayerState<AARPlayerState>()) 
			PlayerState->AddCredits(CreditsPerKill);
	}
}

void AARGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++) {
		if (AARPlayerState* PlayerState = Cast<AARPlayerState>(GameState->PlayerArray[i])) {
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // Single player only
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It) {
		AActor* Actor = *It;

		if (!Actor->Implements<UARInteractInterface>())
			continue;

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetActorTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemoryWriter(ActorData.ByteData);
        FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Convert Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

void AARGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) {
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		for (FActorIterator It(GetWorld()); It; ++It) {
			AActor* Actor = *It;

			if (!Actor->Implements<UARInteractInterface>())
				continue;

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors) {
				if (ActorData.ActorName == Actor->GetName()) {
					Actor->SetActorTransform(ActorData.ActorTransform);

					// Pass the binary array to read from the actor data byte array
					FMemoryReader MemoryReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
					// Find only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					IARInteractInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	} else {
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));
	}
}