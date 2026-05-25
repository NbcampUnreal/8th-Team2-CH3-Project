#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AGameState.generated.h"
	
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageEndDelegate);

UCLASS()
class CH3_TEAM2_API AAGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AAGameState();

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnMasterGameEnd();
	
	UFUNCTION()
	void OnMasterShowStatisticsUI();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	float PlayTime = 0.0f;
	bool bIsTracking = false;
	
	void SaveTime(bool bIsLastStage);
};
