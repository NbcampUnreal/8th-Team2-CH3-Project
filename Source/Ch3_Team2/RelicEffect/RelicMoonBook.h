// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RelicEffect/RelicEffectBase.h"
#include "RelicMoonBook.generated.h"

class UNiagaraSystem;

UCLASS()
class CH3_TEAM2_API URelicMoonBook : public URelicEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginDestroy() override;
	
	virtual void ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic) override;
	
	void SpawnRelic() const;
	
	UPROPERTY(EditAnywhere,Category = "Relic")
	TSubclassOf<AActor> MoonBookBP;
	
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void Heal();
	
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void SpeedUp();
	void SpeedDown();
	
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void AmmoDamageUp();
	void AmmoDamageDown();
	
	void SpawnRelicEffect(int a) const;
	
	//이펙트
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Relic")
	UNiagaraSystem* RelicNiagaraSystemHeal;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Relic")
	UNiagaraSystem* RelicNiagaraSystemDamage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Relic")
	UNiagaraSystem* RelicNiagaraSystemSpeedUp;
protected:
	
	FTimerHandle SpeedUpTimer;
	FTimerHandle DamageTimer;
	FTimerHandle MainTimer;
	
	
	
};
