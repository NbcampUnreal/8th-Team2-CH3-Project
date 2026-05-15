#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

class USaveData;
struct FMonsterKillReport;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyChanged, int32, NewAmount, int32, Delta);

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
	
	void OnStageCleared(int32 StageIndex, float ClearTime);

	UPROPERTY()
	FOnCurrencyChanged OnCurrencyChanged;
	
	// BattleSubsystem Delegate
	UFUNCTION()
	void OnBattleResultReceived(const TArray<FMonsterKillReport>& KillReports, int32 GlobalTotalDamage);
	
private:
	UPROPERTY()
	USaveData* CurrentSave = nullptr;

	static const FString SlotName;
};
