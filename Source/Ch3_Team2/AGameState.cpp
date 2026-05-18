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
	
	// 메인 메뉴일 경우 시간 측정 X
	ULevelFlowSubsystem* LevelFlow = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>();
	if (!LevelFlow || LevelFlow->GetCurrentLevelIndex() == 0)
	{
		SetActorTickEnabled(false);
		return;
	}
	
	RemainingTime = StageDuration;

	GetWorldTimerManager().SetTimer(
		StageTimerHandle,
		this,
		&AAGameState::OnStageTimerExpired,
		StageDuration,
		false
	);
}

void AAGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RemainingTime = FMath::Max(0.f, RemainingTime - DeltaTime);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Yellow,
			FString::Printf(TEXT("Stage Time: %.1f"), RemainingTime));
}

void AAGameState::OnStageTimerExpired()
{
	ULevelFlowSubsystem* LevelFlow = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>();
	if (!LevelFlow)
	{
		return;
	}

	// 마지막 스테이지 여부 체크
	if (LevelFlow->IsLastLevel())
	{
		UBattleSubsystem* BattleSubsystem = GetGameInstance()->GetSubsystem<UBattleSubsystem>();
		if (!BattleSubsystem) return;
		// 배틀 시스템에서 전투 리포트 출력
		BattleSubsystem->BroadcastBattleResult();
		UE_LOG(LogTemp, Warning, TEXT("[StageGameState] 마지막 스테이지 종료 → 메인 레벨로 복귀"));
		OnStageEnd.Broadcast();
	}
	else
	{
		LevelFlow->TravelToNextLevel();
	}
}

void AAGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (StageTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(StageTimerHandle);
	}
}