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

		// 스테이지 자동 전환 대기 시간 (초)
		UPROPERTY(EditDefaultsOnly, Category = "Stage")
		float StageDuration = 10.f;
	
		UPROPERTY(BlueprintAssignable, Category = "GameFlow")
		FOnStageEndDelegate OnStageEnd;
	
	protected:
		virtual void BeginPlay() override;
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	private:
		float RemainingTime = 0.f;
		FTimerHandle StageTimerHandle;

		void OnStageTimerExpired();
};
