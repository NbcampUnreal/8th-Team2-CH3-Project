// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicEecovery.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API URelicEecovery : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	void Healing();

	
protected:
	
	FTimerHandle EcoveryTimer;
};
