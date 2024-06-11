#include "AI/ARControllerAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AARControllerAI::AARControllerAI()
{
}

void AARControllerAI::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
		RunBehaviorTree(BehaviorTree);
}