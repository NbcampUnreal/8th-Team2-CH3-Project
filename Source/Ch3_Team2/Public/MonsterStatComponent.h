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
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InInstigater, AActor* Causer);
	
	EMonsterGrade GetMonsterTag() const { return Stats.MonsterGrade; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAttackRange() const {return Stats.AttackRange;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAttackRate() const {return Stats.AttackRate;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAttackDamage() const {return Stats.AttackDamage;}
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	UFUNCTION()
	void SetDead(bool Dead){bIsDead=Dead;};
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetCurrentHP() const { return static_cast<float>(Stats.CurrentHP); }
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetMaxHP() const { return static_cast<float>(Stats.MaxHP); }
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathDelegate OnDeath;

	//state tree hit 확인용
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bIsHit;
	

protected:
	UPROPERTY(VisibleAnywhere,Category = "Stats")
	FMonsterStats Stats;
	
	UPROPERTY(VisibleAnywhere,Category = "Stats")
	bool bIsDead = false;

};
