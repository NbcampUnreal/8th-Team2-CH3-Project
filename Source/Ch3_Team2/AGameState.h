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
	
		UPROPERTY(BlueprintAssignable, Category = "GameFlow")
		FOnStageEndDelegate OnStageEnd;
	
		virtual void Tick(float DeltaTime) override;

	protected:
		virtual void BeginPlay() override;
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
