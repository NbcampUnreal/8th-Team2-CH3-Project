// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicGrade.h"
#include "Engine/DataTable.h"
#include "RelicData.generated.h"

class URelicEffectBase;

USTRUCT(BlueprintType)
struct FRelicData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName RelicId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RelicName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERelicGrade Grade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERelicStatType RelicStat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URelicEffectBase> EffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability = 0.f;
	
};



