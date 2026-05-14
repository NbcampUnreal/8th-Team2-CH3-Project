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

	UFUNCTION(BlueprintCallable, Category = "Level Flow")
	int32 GetCurrentLevelIndex() const { return CurrentLevelIndex; }
	
	UFUNCTION(BlueprintCallable, Category = "Level Flow")
	void TravelToNextLevel();
	
	UFUNCTION(BlueprintCallable, Category = "Level Flow")
	bool IsLastLevel() const;

protected:
	UPROPERTY(EditDefaultsOnly, Config, Category = "Level Flow")
	TSoftObjectPtr<ULevelFlowDataAsset> LevelFlowData;

private:
	int32 CurrentLevelIndex = INDEX_NONE;

	void SyncCurrentLevelIndex();
	
	UPROPERTY()
	TObjectPtr<ULevelFlowDataAsset> LoadedFlowData;
};