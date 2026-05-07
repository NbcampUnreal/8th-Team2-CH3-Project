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

    if (LoadedFlowData->LevelSequence.IsValidIndex(NextIndex))
        TravelToLevelByIndex(NextIndex);
    else
        TravelToLevelByIndex(0);
}

void ULevelFlowSubsystem::TravelToLevelByIndex(int32 LevelIndex)
{
    if (!LoadedFlowData) return;
    if (!LoadedFlowData->LevelSequence.IsValidIndex(LevelIndex)) return;

    CurrentLevelIndex = LevelIndex;
    OpenLevelByWorld(LoadedFlowData->LevelSequence[LevelIndex]);
}

void ULevelFlowSubsystem::OpenLevelByWorld(const TSoftObjectPtr<UWorld>& WorldRef)
{
    if (WorldRef.IsNull()) return;

    const FName LevelName = FName(*FPackageName::GetShortName(WorldRef.GetLongPackageName()));
    UGameplayStatics::OpenLevel(this, LevelName);
}

void ULevelFlowSubsystem::SyncCurrentLevelIndex()
{
    if (!LoadedFlowData) return;

    UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
    if (!World) return;

    FString CleanMapName = World->GetMapName();
    CleanMapName.RemoveFromStart(World->StreamingLevelsPrefix);

    for (int32 i = 0; i < LoadedFlowData->LevelSequence.Num(); ++i)
    {
        const auto& LevelRef = LoadedFlowData->LevelSequence[i];
        if (LevelRef.IsNull()) continue;

        if (FPackageName::GetShortName(LevelRef.GetLongPackageName()) == CleanMapName)
        {
            CurrentLevelIndex = i;
            return;
        }
    }
}