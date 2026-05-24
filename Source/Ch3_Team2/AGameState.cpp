#include "AGameState.h"

#include "Data/LevelFlowSubsystem.h"
#include "Data/MasterSubsystem.h"

AAGameState::AAGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAGameState::BeginPlay()
{
	Super::BeginPlay();
	
	bIsTracking = true;
}

void AAGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsTracking)
	{
		PlayTime += DeltaTime;
	}
}

void AAGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bIsTracking = false;
	
	if (EndPlayReason == EEndPlayReason::EndPlayInEditor)
	{
		return;
	}

	auto* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
	if(!MasterSubsystem)
	{
		return;
	}
	
	if (auto* LevelFlowSubsystem = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>())
	{
		if (LevelFlowSubsystem->GetPrevLevelIndex() != 0)
		{
			MasterSubsystem->OnSaveTime.Broadcast(LevelFlowSubsystem->GetPrevLevelIndex() - 1, PlayTime);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}
