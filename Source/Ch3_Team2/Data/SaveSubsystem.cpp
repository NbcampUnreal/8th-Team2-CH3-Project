#include "Data/SaveSubsystem.h"
#include "MasterSubsystem.h"
#include "Data/SaveData.h"
#include "Kismet/GameplayStatics.h"

FString USaveSubsystem::GetSlotName() const
{
    FDateTime Now = FDateTime::Now();
    return FString::Printf(TEXT("Save_%04d%02d%02d_%02d%02d%02d"),
        Now.GetYear(), Now.GetMonth(), Now.GetDay(),
        Now.GetHour(), Now.GetMinute(), Now.GetSecond()
    );
    // 결과 예시: Save_20250524_143022
}

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    // 순서 꼬임 방지
    Collection.InitializeDependency<UMasterSubsystem>();

    Super::Initialize(Collection);
    
    LoadGame();
    
    MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
    if (MasterSubsystem)
    {
        MasterSubsystem->OnSaveRelic.AddDynamic(this, &USaveSubsystem::OnMasterSaveRelic);
        MasterSubsystem->OnSavePlayer.AddDynamic(this, &USaveSubsystem::OnMasterSavePlayer);
        MasterSubsystem->OnSaveGun.AddDynamic(this, &USaveSubsystem::OnMasterSaveGun);
        MasterSubsystem->OnBattleResult.AddDynamic(this, &USaveSubsystem::OnMasterBattleResult);
        MasterSubsystem->OnSaveTime.AddDynamic(this, &USaveSubsystem::OnMasterSaveTime);
        MasterSubsystem->OnShowStatisticsUI.AddDynamic(this, &USaveSubsystem::OnMasterShowStatisticsUI);
    }
}

void USaveSubsystem::Deinitialize()
{
    if (MasterSubsystem)
    {
        MasterSubsystem->OnShowStatisticsUI.RemoveDynamic(this, &USaveSubsystem::OnMasterShowStatisticsUI);
        MasterSubsystem->OnSaveTime.RemoveDynamic(this, &USaveSubsystem::OnMasterSaveTime);
        MasterSubsystem->OnBattleResult.RemoveDynamic(this, &USaveSubsystem::OnMasterBattleResult);
        MasterSubsystem->OnSaveGun.RemoveDynamic(this, &USaveSubsystem::OnMasterSaveGun);
        MasterSubsystem->OnSavePlayer.RemoveDynamic(this, &USaveSubsystem::OnMasterSavePlayer);
        MasterSubsystem->OnSaveRelic.RemoveDynamic(this, &USaveSubsystem::OnMasterSaveRelic);
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
    UGameplayStatics::SaveGameToSlot(CurrentSave, CurrentSlotName, 0);
}

void USaveSubsystem::LoadGame()
{
    CurrentSave = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
}

void USaveSubsystem::OnMasterSaveRelic(TArray<int32> RelicIDs)
{
    if (!CurrentSave)
    {
        return;
    }
    
    CurrentSave->RelicIDs = RelicIDs;
}

void USaveSubsystem::OnMasterSavePlayer(int32 PlayerLevel, int32 PlayerSkill, int32 PlayerWeapon)
{
    if (!CurrentSave)
    {
        return;
    }
    
    CurrentSave->PlayerLevel = PlayerLevel;
    CurrentSave->PlayerSkill = PlayerSkill;
    CurrentSave->PlayerWeapon = PlayerWeapon;
}

void USaveSubsystem::OnMasterSaveGun(int32 GripLevel, int32 ScopeLevel, int32 MagazineLevel, int32 BulletLevel)
{
    if (!CurrentSave)
    {
        return;
    }
    
    CurrentSave->GripLevel     = GripLevel;
    CurrentSave->ScopeLevel    = ScopeLevel;
    CurrentSave->MagazineLevel = MagazineLevel;
    CurrentSave->BulletLevel   = BulletLevel;
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
    
    UE_LOG(LogTemp, Warning, TEXT("보스 %d 마리 죽임"), BossKills);
}

void USaveSubsystem::OnMasterSaveTime(int32 StageIndex, float ClearTime)
{
    if (!CurrentSave)
    {
        return;
    }
    
    if (!CurrentSave->StageClearTime.IsValidIndex(StageIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("잘못된 StageIndex: %d"), StageIndex);
        return;
    }

    CurrentSave->StageClearTime[StageIndex] = ClearTime;
}

void USaveSubsystem::OnMasterShowStatisticsUI()
{
    UE_LOG(LogTemp, Warning, TEXT("게임 저장 시점"));
    CurrentSlotName = GetSlotName();
    SaveGame();
    
    UE_LOG(LogTemp, Warning, TEXT("UI 뜨는 시점"));
    OnShowStatistics.Broadcast();
}
