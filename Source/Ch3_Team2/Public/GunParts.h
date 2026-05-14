// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunParts.generated.h"

UENUM(BlueprintType)
enum class EPartsName : uint8
{
	eNone,
	eBullet UMETA(DisplayName = "Bullet"),
	eMagazine UMETA(DisplayName = "Magazine"),
	eScope UMETA(DisplayName = "Scope"),
	eHandle UMETA(DisplayName = "Handle")
};

USTRUCT(BlueprintType)
struct FGunParts
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	// 파츠 이름
	FString Name = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	// 파츠별 레벨
	int32 Level = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	// 파츠 기능 수치
	float Value = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun_Parts")
	EPartsName Parts =EPartsName::eNone;
};
