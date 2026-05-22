// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MonsterTypes.h"
#include "MasterSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FMonsterKillReport
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EMonsterGrade MonsterGrade;

	UPROPERTY(BlueprintReadOnly)
	int32 KillCount;

	FMonsterKillReport() : MonsterGrade(EMonsterGrade::None), KillCount(0) {}
	FMonsterKillReport(EMonsterGrade InGrade, int32 InCount) : MonsterGrade(InGrade), KillCount(InCount) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleSummarySignature, 
	const TArray<FMonsterKillReport>&, KillReports, 
	int32, GlobalTotalDamage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyChanged, int32, NewAmount, int32, Delta);

UCLASS()
class CH3_TEAM2_API UMasterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 방송 채널들
	UPROPERTY()
	FOnBattleSummarySignature OnBattleResult;
	UPROPERTY()
	FOnCurrencyChanged OnCurrencyChanged;
};
