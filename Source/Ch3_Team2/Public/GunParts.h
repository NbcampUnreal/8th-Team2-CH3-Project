// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunParts.generated.h"

USTRUCT(BlueprintType)
struct FGunParts
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	// 파츠 이름
	FString PartsName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	// 파츠별 레벨
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	// 파츠 기능 수치
	float Value;
};
