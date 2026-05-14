#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AGameState.generated.h"

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
	
	protected:
		virtual void BeginPlay() override;

	private:
		float RemainingTime = 0.f;
		FTimerHandle StageTimerHandle;

		void OnStageTimerExpired();
};
