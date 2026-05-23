#include "Data/LevelFlowSubsystem.h"
#include "LevelFlowDataAsset.h"
#include "Kismet/GameplayStatics.h"

void ULevelFlowSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (LevelFlowData.IsNull())
    {
        return;
    }

    LoadedFlowData = LevelFlowData.LoadSynchronous();
    SyncCurrentLevelIndex();
}

void ULevelFlowSubsystem::TravelToNextLevel()
{
    if (!LoadedFlowData)
    {
        return;
    }

    if (LoadedFlowData->Levels.IsValidIndex(CurrentLevelIndex + 1))
    {
        TravelToLevelByIndex(CurrentLevelIndex + 1);
    }
    else
    {
        TravelToLevelByIndex(0);
    }
}

void ULevelFlowSubsystem::TravelToLevelByIndex(int32 LevelIndex)
{
    if (!LoadedFlowData)
    {
        return;
    }
    
    if (!LoadedFlowData->Levels.IsValidIndex(LevelIndex))
    {
        return;
    }

    const TSoftObjectPtr<UWorld>& LevelRef = LoadedFlowData->Levels[LevelIndex];
    if (LevelRef.IsNull())
    {
        return;
    }

    PrevLevelIndex = CurrentLevelIndex;
    CurrentLevelIndex = LevelIndex;

    const FName LevelName = FName(*FPackageName::GetShortName(LevelRef.GetLongPackageName()));
    UGameplayStatics::OpenLevel(this, LevelName);
}

void ULevelFlowSubsystem::SyncCurrentLevelIndex()
{
    if (!LoadedFlowData)
    {
        return;
    }

    UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
    if (!World)
    {
        return;
    }

    FString CleanMapName = World->GetMapName();
    CleanMapName.RemoveFromStart(World->StreamingLevelsPrefix);

    for (int32 i = 0; i < LoadedFlowData->Levels.Num(); ++i)
    {
        const auto& LevelRef = LoadedFlowData->Levels[i];
        if (LevelRef.IsNull())
        {
            continue;
        }

        if (FPackageName::GetShortName(LevelRef.GetLongPackageName()) == CleanMapName)
        {
            CurrentLevelIndex = i;
            return;
        }
    }
}