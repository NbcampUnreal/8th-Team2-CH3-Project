// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicMoonRabbit.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API URelicMoonRabbit : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	UPROPERTY(EditAnywhere, Category = "StaticMesh")
	TSubclassOf<AActor> MoonRabbitEffectActor;
	
	void SpawnMoonRabbitEffect() const;
	
private:
	
	FTimerHandle SpawnMoonRabbitEffectTimer;
	
};
