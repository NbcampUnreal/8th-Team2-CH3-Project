// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnBattleResultSignature,
	AActor*, Attacker,
	AActor*, Victim,
	float, FinalDamage,
	bool, bIsCritical);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API UBattleSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattleSystemComponent();

	UPROPERTY(BlueprintAssignable, Category = "Battle")
	FOnBattleResultSignature OnBattleResult;

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier);
	
protected:
	virtual void BeginPlay() override;
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	

	
};
