// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicReferee.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API URelicReferee : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	void SpawnRefereeEffect() const;
	
	UPROPERTY(EditAnywhere, Category = "Referee Blueprint")
	TSubclassOf<AActor> RefereeBP;
	
	
	
protected:
	
	FTimerHandle RelicRefereeTimerHandle;
	
};
