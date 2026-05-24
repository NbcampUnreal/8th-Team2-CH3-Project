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
        MasterSubsystem->OnBattleResult.AddDynamic(this, &USaveSubsystem::OnMasterBattleResult);
        MasterSubsystem->OnSaveTime.AddDynamic(this, &USaveSubsystem::OnMasterSaveTime);
    }
}

void USaveSubsystem::Deinitialize()
{
    if (MasterSubsystem)
    {
        MasterSubsystem->OnSaveTime.RemoveDynamic(this, &USaveSubsystem::OnMasterSaveTime);
        MasterSubsystem->OnBattleResult.RemoveDynamic(this, &USaveSubsystem::OnMasterBattleResult);
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

void USaveSubsystem::OnMasterBattleResult(int32 MeleeKills, int32 RangedKills, int32 EliteMeleeKills, int32 EliteRangedKills, int32 BossKills, int32 GlobalTotalDamage)
{
    if (!CurrentSave)
    {
        return;
    }
    
    CurrentSave->TotalDamage        = GlobalTotalDamage;
    
    CurrentSave->MeleeKills         = MeleeKills;
    CurrentSave->RangedKills        = RangedKills;
    CurrentSave->EliteMeleeKills    = EliteMeleeKills;
    CurrentSave->EliteRangedKills   = EliteRangedKills;
    CurrentSave->BossKills          = BossKills;
}

void USaveSubsystem::OnMasterSaveTime(int32 StageIndex, float ClearTime)
{
    if (!CurrentSave)
    {
        return;
    }
    
    if (ClearTime <= 0.f)
    {
        return;
    }
    
    if (!CurrentSave->StageClearTime.IsValidIndex(StageIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("잘못된 StageIndex: %d"), StageIndex);
        return;
    }

    CurrentSave->StageClearTime[StageIndex] = ClearTime;
    
    SaveGame();
}
