#include "AGameState.h"

#include "Data/LevelFlowSubsystem.h"
#include "Data/MasterSubsystem.h"
#include "Kismet/GameplayStatics.h"

AAGameState::AAGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if (UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>())
	{
		MasterSubsystem->OnGameEnd.AddDynamic(this, &AAGameState::OnMasterGameEnd);
		MasterSubsystem->OnShowStatisticsUI.AddDynamic(this, &AAGameState::OnMasterShowStatisticsUI);
	}
	
	bIsTracking = true;
}

void AAGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>())
	{
		MasterSubsystem->OnGameEnd.RemoveDynamic(this, &AAGameState::OnMasterGameEnd);
		MasterSubsystem->OnShowStatisticsUI.RemoveDynamic(this, &AAGameState::OnMasterShowStatisticsUI);
	}
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	
	// 에디터에서 종료시 EndPlay가 한번 더 호출 되어서, 세이브 파일이 덮어쓰기 되는 문제 방지
	if (EndPlayReason != EEndPlayReason::EndPlayInEditor)
	{
		SaveTime(false);
	}
	
	Super::EndPlay(EndPlayReason);
}

void AAGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsTracking)
	{
		PlayTime += DeltaTime;
	}
}

void AAGameState::SaveTime(bool bIsLastStage)
{
	bIsTracking = false;

	auto* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
	auto* LevelFlowSubsystem = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>();
	if (!MasterSubsystem || !LevelFlowSubsystem)
	{
		return;
	}

	const int32 PrevIndex = LevelFlowSubsystem->GetPrevLevelIndex();
	if (PrevIndex == 0)
	{
		return;
	}
	
	if (bIsLastStage == true)
	{
		MasterSubsystem->OnSaveTime.Broadcast(PrevIndex, PlayTime);
	}
	else
	{
		MasterSubsystem->OnSaveTime.Broadcast(PrevIndex - 1, PlayTime);
	}
}

void AAGameState::OnMasterGameEnd()
{
	SaveTime(true);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);
}

void AAGameState::OnMasterShowStatisticsUI()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
