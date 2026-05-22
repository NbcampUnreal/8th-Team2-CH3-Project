#include "Data/SaveSubsystem.h"
#include "MasterSubsystem.h"
#include "Data/SaveData.h"
#include "Kismet/GameplayStatics.h"

const FString USaveSubsystem::SlotName = TEXT("MainSave");

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    // 순서 꼬임 방지
    Collection.InitializeDependency<UMasterSubsystem>();

    Super::Initialize(Collection);
    
    LoadGame();
    
    MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
    if (MasterSubsystem)
    {
        MasterSubsystem->OnBattleResult.AddDynamic(this, &USaveSubsystem::OnBattleResultReceived);
    }
}

void USaveSubsystem::Deinitialize()
{
    if (MasterSubsystem)
    {
        MasterSubsystem->OnBattleResult.RemoveDynamic(this, &USaveSubsystem::OnBattleResultReceived);
    }

    // 부모를 마지막에 해제
    Super::Deinitialize();
}

void USaveSubsystem::SaveGame()
{
    if (!CurrentSave)
    {
        return;
    }
    UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
}

void USaveSubsystem::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        CurrentSave = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    }

    if (!CurrentSave)
    {
        CurrentSave = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
    }
}

void USaveSubsystem::AddCurrency(int32 Amount)
{
    if (!CurrentSave)
    {
        return;
    }
    
    CurrentSave->Currency += Amount;
    if (MasterSubsystem)
    {
        MasterSubsystem->OnCurrencyChanged.Broadcast(CurrentSave->Currency, Amount);
    }
}

int32 USaveSubsystem::GetCurrency() const
{
    return CurrentSave ? CurrentSave->Currency : 0;
}

bool USaveSubsystem::TrySpendCurrency(int32 Amount)
{
    if (!CurrentSave || CurrentSave->Currency < Amount)
    {
        return false;
    }

    CurrentSave->Currency -= Amount;
    if (MasterSubsystem)
    {
        MasterSubsystem->OnCurrencyChanged.Broadcast(CurrentSave->Currency, Amount);
    }
    return true;
}

void USaveSubsystem::OnBattleResultReceived(const TArray<FMonsterKillReport>& KillReports, int32 GlobalTotalDamage)
{
    if (!CurrentSave)
    {
        return;
    }
    
    CurrentSave->TotalDamage += GlobalTotalDamage;
    
    for (const FMonsterKillReport& Report : KillReports)
    {
        switch (Report.MonsterGrade)
        {
            case EMonsterGrade::Melee:
                CurrentSave->MeleeKills += Report.KillCount;
                break;
            case EMonsterGrade::Ranged:
                CurrentSave->RangedKills += Report.KillCount;
                break;
            case EMonsterGrade::EliteMelee:
                CurrentSave->EliteMeleeKills += Report.KillCount;
                break;
            case EMonsterGrade::EliteRanged:
                CurrentSave->EliteRangedKills += Report.KillCount;
                break;
            case EMonsterGrade::Boss:
                CurrentSave->BossKills += Report.KillCount;
                break;
            
            default:
                break;
        }
    }
}

// TODO: 모든 스테이지 종료시 클리어 타임 저장
void USaveSubsystem::OnStageCleared(int32 StageIndex, float ClearTime)
{
    if (!CurrentSave)
    {
        return;
    }

    auto UpdateBest = [](float& Slot, float NewTime)
    {
        if (NewTime <= 0.f)
        {
            return;
        }
        if (Slot <= 0.f || NewTime < Slot)
        {
            Slot = NewTime;
        }
    };

    switch (StageIndex)
    {
        case 1: UpdateBest(CurrentSave->Stage1Time, ClearTime); break;
        case 2: UpdateBest(CurrentSave->Stage2Time, ClearTime); break;
        case 3: UpdateBest(CurrentSave->Stage3Time, ClearTime); break;
        default: break;
    }

    SaveGame();
}
