// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/MasterSubsystem.h"
#include "BattleSubsystem.generated.h"

UCLASS()
class CH3_TEAM2_API UBattleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier);
	float CalculateFinalDamage(AActor* Attacker, float BaseDamage, bool bIsCritical, float CritMultiplier);
	void ProcessDeathAndKillCount(AActor* Victim);
	void BroadcastBattleResult();

private:
	int32 TotalDamage = 0;
	TArray<FMonsterKillReport> Reports;
};
