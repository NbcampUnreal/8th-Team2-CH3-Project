// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicProbability.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API URelicProbability : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	bool bIsGainedHPRelic = false;
	bool bIsGainedDamageRelic = false;
	
	void AddStatProbability() const;
	
protected:
	
	UFUNCTION(blueprintCallable , Category = "RelicEffect")
	void DeathMonster();
	
};
