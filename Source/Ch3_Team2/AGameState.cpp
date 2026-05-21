#include "AGameState.h"

#include "Battle/BattleSubsystem.h"
#include "Data/LevelFlowSubsystem.h"

AAGameState::AAGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AAGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
