// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "NiagaraSystem.h"
#include "RelicData.h"
#include "RelicLightning.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API URelicLightning : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	void SpawnLightning();
	
	//이펙트 에셋
	UPROPERTY(EditAnywhere, Category = "Lightning")
	UNiagaraSystem* LightningFX;
	
private:
	
	FTimerHandle LightningTimerHandle;
};
