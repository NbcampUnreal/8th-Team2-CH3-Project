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

	UFUNCTION(BlueprintCallable, Category = "Level Flow")
	void TravelToNextLevel();

	void TravelToLevelByIndex(int32 LevelIndex);

protected:
	UPROPERTY(EditDefaultsOnly, Config, Category = "Level Flow")
	TSoftObjectPtr<ULevelFlowDataAsset> LevelFlowData;

private:
	UPROPERTY()
	TObjectPtr<ULevelFlowDataAsset> LoadedFlowData;

	int32 CurrentLevelIndex = INDEX_NONE;

	void SyncCurrentLevelIndex();
};