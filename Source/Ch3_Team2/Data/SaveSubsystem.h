#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

struct FRelicData;
class UMasterSubsystem;
class USaveData;

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
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SaveGame();
	void LoadGame();
	
	TArray<int32> GetRelicIDs() const { return CurrentSave->RelicIDs; };

	int32 GetPlayerLevel()  const { return CurrentSave->PlayerLevel; }
	int32 GetPlayerSkill()  const { return CurrentSave->PlayerSkill; }
	int32 GetPlayerWeapon() const { return CurrentSave->PlayerWeapon; }
	
	int32 GetGripLevel()  const { return CurrentSave->GripLevel; }
	int32 GetScopeLevel()  const { return CurrentSave->ScopeLevel; }
	int32 GetMagazineLevel() const { return CurrentSave->MagazineLevel; }
	int32 GetBulletLevel()  const { return CurrentSave->BulletLevel; }

private:
	UPROPERTY()
	USaveData* CurrentSave = nullptr;
	
	FString CurrentSlotName;
	FString GetSlotName() const;
};
