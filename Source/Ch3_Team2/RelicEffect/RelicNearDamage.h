// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RelicData.h"
#include "RelicEffectBase.h"
#include "RelicNearDamage.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CH3_TEAM2_API URelicNearDamage : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyRelic(UObject* WorldContextObject,const FRelicData& NewRelic) override;
	void DamageNearByEnemies() const;
	
private:
	
	FTimerHandle GetDamageNearByEnemiesTimer;
	//공격 범위
	float Radius = 500.f;
};
