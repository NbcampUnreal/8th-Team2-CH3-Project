// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RelicGrade.h"
#include "RelicApplyManager.generated.h"

class RelicGrade;
class UNiagaraSystem;

UCLASS()
class CH3_TEAM2_API ARelicApplyManager : public AActor
{
	GENERATED_BODY()
	
public:

	void ApplyRelicById(TArray<TPair<int32,bool>> &RelicIDs);
	
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void RelicStatUp(float Value, ERelicStatType StatType) const;
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void RelieBattleSystem(float Value, AActor* Victim);
	
	void FindRelicData(int32 RelicID) const;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool Revive();
	
	
	void Relic1027();
	void Relic1117();
	void Relic1118();
	void Relic1119();
	void Relic1120();
	void Relic1121();
	void Relic1122();
	void Relic1123();
	void Relic1124();
	void Relic1125();
	
	
	
	UPROPERTY(EditAnywhere, Category = "RelicBP")
	TSubclassOf<AActor> ShootingStar;
	UPROPERTY(EditAnywhere, Category = "RelicBP")
	TSubclassOf<AActor> MoonBook;
	UPROPERTY(EditAnywhere, Category = "RelicBP")
	TSubclassOf<AActor> Referee;
	UPROPERTY(EditAnywhere, Category = "RelicBP")
	TSubclassOf<AActor> MoonRabbit;
	
	
	
	UPROPERTY(EditAnywhere, Category = "RelicNiagara")
	UNiagaraSystem* LightningFX;
	UPROPERTY(EditAnywhere, Category = "RelicNiagara")
	UNiagaraSystem* Aurora;

	
protected:

	UPROPERTY(EditAnywhere, Category = "RelicData")
	UDataTable* RelicDataTable;
	
	FTimerHandle TimerHandle1027;
	FTimerHandle TimerHandle1117;
	FTimerHandle TimerHandle1118;
	FTimerHandle TimerHandle1120;
	FTimerHandle TimerHandle1123;
	FTimerHandle TimerHandle1124;
	

};
