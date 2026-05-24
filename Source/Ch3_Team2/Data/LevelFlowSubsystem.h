#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelFlowSubsystem.generated.h"

class ULevelFlowDataAsset;

UCLASS(Config=Game)
class CH3_TEAM2_API ULevelFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void TravelToLevelByIndex(int32 LevelIndex);
	int32 GetCurrentLevelIndex() const { return CurrentLevelIndex; }
	int32 GetPrevLevelIndex() const { return PrevLevelIndex; }
	
	UFUNCTION(BlueprintCallable, Category = "Level Flow")
	void TravelToNextLevel();
	
protected:
	UPROPERTY(EditDefaultsOnly, Config, Category = "Level Flow")
	TSoftObjectPtr<ULevelFlowDataAsset> LevelFlowData;

private:
	int32 CurrentLevelIndex = INDEX_NONE;
	int32 PrevLevelIndex = INDEX_NONE;

	void SyncCurrentLevelIndex();
	
	UPROPERTY()
	TObjectPtr<ULevelFlowDataAsset> LoadedFlowData;
};