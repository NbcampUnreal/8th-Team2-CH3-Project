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
	int32 RelicId = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RelicName = "Name";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERelicGrade Grade = ERelicGrade::Common;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERelicStatType RelicStat = ERelicStatType::MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* RelicImage;
	
};



