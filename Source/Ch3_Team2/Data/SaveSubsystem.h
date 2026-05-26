#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

struct FRelicData;
class UMasterSubsystem;
class USaveData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowStatistics);

UCLASS()
class CH3_TEAM2_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UMasterSubsystem* MasterSubsystem;

	UFUNCTION()
	void OnMasterBattleResult(
		int32 MeleeKills, int32 RangedKills,
		int32 EliteMeleeKills, int32 EliteRangedKills,
		int32 BossKills, int32 GlobalTotalDamage
	);

	UFUNCTION()
	void OnMasterSaveTime(int32 StageIndex, float ClearTime);
	
	UFUNCTION()
	void OnMasterSaveRelic(TArray<int32> RelicIDs);
	
	UFUNCTION()
	void OnMasterSavePlayer(int32 PlayerLevel, int32 PlayerSkill, int32 PlayerWeapon);
	
	UFUNCTION()
	void OnMasterSaveGun(int32 GripLevel, int32 ScopeLevel, int32 MagazineLevel, int32 BulletLevel);
	
	UFUNCTION()
	void OnMasterShowStatisticsUI();

	// --- Relic ---
	UFUNCTION(BlueprintPure)
	TArray<int32> GetRelicIDs() const { return CurrentSave ? CurrentSave->RelicIDs : TArray<int32>(); }

	// --- Player ---
	UFUNCTION(BlueprintPure)
	int32 GetPlayerLevel() const { return CurrentSave ? CurrentSave->PlayerLevel : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetPlayerSkill() const { return CurrentSave ? CurrentSave->PlayerSkill : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetPlayerWeapon() const { return CurrentSave ? CurrentSave->PlayerWeapon : 0; }

	// --- Gun ---
	UFUNCTION(BlueprintPure)
	int32 GetGripLevel() const { return CurrentSave ? CurrentSave->GripLevel : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetScopeLevel() const { return CurrentSave ? CurrentSave->ScopeLevel : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetMagazineLevel() const { return CurrentSave ? CurrentSave->MagazineLevel : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetBulletLevel() const { return CurrentSave ? CurrentSave->BulletLevel : 0; }

	// --- Stage Clear Time ---
	UFUNCTION(BlueprintPure)
	float GetStageClearTime() const { 
		return	CurrentSave->StageClearTime[0] + 
				CurrentSave->StageClearTime[1] + 
				CurrentSave->StageClearTime[2]; 
	}

	UFUNCTION(BlueprintPure)
	float GetStageClearTimeAt(int32 StageIndex) const
	{
	    if (!CurrentSave) return 0.f;
	    if (!CurrentSave->StageClearTime.IsValidIndex(StageIndex)) return 0.f;
	    return CurrentSave->StageClearTime[StageIndex];
	}

	// --- Kill Stats ---
	UFUNCTION(BlueprintPure)
	int32 GetMeleeKills() const { return CurrentSave ? CurrentSave->MeleeKills : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetRangedKills() const { return CurrentSave ? CurrentSave->RangedKills : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetEliteMeleeKills() const { return CurrentSave ? CurrentSave->EliteMeleeKills : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetEliteRangedKills() const { return CurrentSave ? CurrentSave->EliteRangedKills : 0; }

	UFUNCTION(BlueprintPure)
	int32 GetBossKills() const { return CurrentSave ? CurrentSave->BossKills : 0; }

	// --- Damage ---
	UFUNCTION(BlueprintPure)
	int32 GetTotalDamage() const { return CurrentSave ? CurrentSave->TotalDamage : 0; }

	UPROPERTY(BlueprintAssignable)
	FOnShowStatistics OnShowStatistics;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	USaveData* CurrentSave = nullptr;
	
	FString CurrentSlotName;
	FString GetSlotName() const;
	
	void SaveGame();
	void LoadGame();
};
