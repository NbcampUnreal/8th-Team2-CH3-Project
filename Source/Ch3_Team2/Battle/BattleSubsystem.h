// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BattleSubsystem.generated.h"

class AAPlayer;

UCLASS()
class CH3_TEAM2_API UBattleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier);
	
	float CalculateFinalDamage(AActor* Attacker, float BaseDamage, bool bIsCritical, float CritMultiplier);
	
	void ProcessDeathAndKillCount(AActor* Victim);
	
	UFUNCTION()
	void BroadcastBattleResult();

protected:
	TWeakObjectPtr<AAPlayer> CachedPlayer;
	
private:
	int32 TotalDamage = 0;
	int32 MeleeKills = 0;
	int32 RangedKills = 0;
	int32 EliteMeleeKills = 0;
	int32 EliteRangedKills = 0;
	int32 BossKills = 0;
};
