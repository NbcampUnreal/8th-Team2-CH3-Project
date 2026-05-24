#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

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

	void AddCurrency(int32 Amount);
	int32 GetCurrency() const;
	bool TrySpendCurrency(int32 Amount);

	UPROPERTY()
	UMasterSubsystem* MasterSubsystem;	
	
	UFUNCTION()
	void OnMasterBattleResult(
		int32 MeleeKills,		int32 RangedKills,
		int32 EliteMeleeKills,	int32 EliteRangedKills,
		int32 BossKills,		int32 GlobalTotalDamage
	);
	
	UFUNCTION()
	void OnMasterSaveTime(int32 StageIndex, float ClearTime);
	
private:
	UPROPERTY()
	USaveData* CurrentSave = nullptr;
	
	static const FString SlotName;
};
