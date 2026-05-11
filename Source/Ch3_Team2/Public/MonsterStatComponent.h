// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterTypes.h"
#include "MonsterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API UMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterStatComponent();
	virtual void BeginPlay() override;
	//스텟 초기화
	void InitializeStats(const FMonsterStats& NewStats);
	
	//데미지 처리
	UFUNCTION()
	void DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InInstigater, AActor* Causer);
	
	
	float GetAttackRange() const;
	float GetAttackRate() const;
	bool IsDead() const;
public:	
	UPROPERTY(BlueprintAssignable)
	FOnHpChangedDelegate OnHpChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDeathDelegate OnDeath;

protected:
	UPROPERTY(VisibleAnywhere,Category = "Stats")
	FMonsterStats Stats;
	
	UPROPERTY(VisibleAnywhere,Category = "Stats")
	bool bIsDead = false;

};
