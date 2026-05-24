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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SaveGame();
	void LoadGame();

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
	void OnMasterSavePlayer(int32 PlayerLevel);
	
	UFUNCTION()
	void OnMasterSaveGun(int32 GripLevel, int32 ScopeLevel, int32 MagazineLevel, int32 BulletLevel);

	int32 GetPlayerLevel() const { return CurrentSave->PlayerLevel; }

private:
	UPROPERTY()
	USaveData* CurrentSave = nullptr;

	static const FString SlotName;
};
