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

	// 에디터에서 종료시 EndPlay가 한번 더 호출 되어서, 세이브 파일이 덮어쓰기 되는 문제 방지
    if (EndPlayReason != EEndPlayReason::EndPlayInEditor)
    {
        BroadcastSaveTime();
    }

    Super::EndPlay(EndPlayReason);
}

void AAGameState::BroadcastSaveTime() const
{
    auto* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
	    return;
    }

    auto* MasterSubsystem = GameInstance->GetSubsystem<UMasterSubsystem>();
    auto* LevelFlowSubsystem = GameInstance->GetSubsystem<ULevelFlowSubsystem>();
    if (!MasterSubsystem || !LevelFlowSubsystem)
    {
	    return;
    }

    const int32 PrevIndex = LevelFlowSubsystem->GetPrevLevelIndex();
    if (PrevIndex == 0)
    {
	    return;
    }

    MasterSubsystem->OnSaveTime.Broadcast(PrevIndex - 1, PlayTime);
}
