// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterTypes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate,AController*,Instigator);

UENUM(BlueprintType)
enum class EMonsterGrade : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Elite UMETA(DisplayName = "Elite"),
	Boss UMETA(DisplayName = "Boss"),
};

USTRUCT(BlueprintType)
struct  FMonsterStats
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	int32 CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	int32 AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	float AttackRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	EMonsterGrade MonsterGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStats")
	int32 Score;
	
	FMonsterStats():MaxHP(100.f),CurrentHP(100.f),AttackDamage(5.f),MaxWalkSpeed(300.f),MovementSpeed(300.f),
	AttackRate(1.5f),AttackRange(100.f),Score(100){};
};
