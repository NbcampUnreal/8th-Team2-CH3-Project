#include "Data/LevelFlowSubsystem.h"
#include "LevelFlowDataAsset.h"
#include "Kismet/GameplayStatics.h"

void ULevelFlowSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (LevelFlowData.IsNull()) return;

    LoadedFlowData = LevelFlowData.LoadSynchronous();
    SyncCurrentLevelIndex();
}

void ULevelFlowSubsystem::TravelToNextLevel()
{
    if (!LoadedFlowData) return;

    const int32 NextIndex = CurrentLevelIndex + 1;

    if (LoadedFlowData->Levels.IsValidIndex(NextIndex))
        TravelToLevelByIndex(NextIndex);
    else
        TravelToLevelByIndex(0);
}

void ULevelFlowSubsystem::TravelToLevelByIndex(int32 LevelIndex)
{
    if (!LoadedFlowData) return;
    if (!LoadedFlowData->Levels.IsValidIndex(LevelIndex)) return;

    const TSoftObjectPtr<UWorld>& LevelRef = LoadedFlowData->Levels[LevelIndex];
    if (LevelRef.IsNull()) return;

    CurrentLevelIndex = LevelIndex;

    const FName LevelName = FName(*FPackageName::GetShortName(LevelRef.GetLongPackageName()));
    UGameplayStatics::OpenLevel(this, LevelName);
}

// 아무 레벨에서 시작해도 대응이 될 수 있게 만들어둠
void ULevelFlowSubsystem::SyncCurrentLevelIndex()
{
    if (!LoadedFlowData) return;

    UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
    if (!World) return;

    FString CleanMapName = World->GetMapName();
    CleanMapName.RemoveFromStart(World->StreamingLevelsPrefix);

    for (int32 i = 0; i < LoadedFlowData->Levels.Num(); ++i)
    {
        const auto& LevelRef = LoadedFlowData->Levels[i];
        if (LevelRef.IsNull()) continue;

        if (FPackageName::GetShortName(LevelRef.GetLongPackageName()) == CleanMapName)
        {
            CurrentLevelIndex = i;
            return;
        }
    }
}