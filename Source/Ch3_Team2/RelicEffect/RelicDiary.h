// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicDiary.generated.h"

class UNiagaraSystem;

UCLASS()
class CH3_TEAM2_API URelicDiary : public URelicEffectBase
{
	GENERATED_BODY()
	
	bool bIsSelected = false;
	
public:
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	UPROPERTY(EditAnywhere, Category = "Relic Effect")
	UNiagaraSystem* RelicEffect;
	
	bool Revive();
	
};
