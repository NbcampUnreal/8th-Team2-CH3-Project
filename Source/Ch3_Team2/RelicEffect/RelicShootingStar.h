// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicShootingStar.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API URelicShootingStar : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	void SpawnRelicBP();
	
	virtual void BeginDestroy() override;
	
	UPROPERTY(EditAnywhere, Category = "Relic")
	TSubclassOf<AActor> RelicEffectBP;
	
protected:
	
	FTimerHandle SpawnTimer;
	
	
};
