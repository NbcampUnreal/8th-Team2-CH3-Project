// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MonsterTypes.h"
#include "BattleSubsystem.generated.h"

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

UCLASS()
class CH3_TEAM2_API UBattleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FOnBattleSummarySignature OnBattleResult;

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier);
	float CalculateFinalDamage(AActor* Attacker, float BaseDamage, bool bIsCritical, float CritMultiplier);
	void ProcessDeathAndKillCount(AActor* Victim);
	void BroadcastBattleResult();

private:
	int32 TotalDamage = 0;
	TMap<EMonsterGrade, int32> GradeKillCounts;
};
