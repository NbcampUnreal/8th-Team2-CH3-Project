// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"

UMonsterStatComponent::UMonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this,&UMonsterStatComponent::DamageTake);
}

void UMonsterStatComponent::InitializeStats(const FMonsterStats& NewStats)
{
	Stats = NewStats;
	Stats.CurrentHP = Stats.MaxHP;
	bIsDead = false;
	
	OnHpChanged.Broadcast(Stats.CurrentHP,Stats.MaxHP,0);
}

void UMonsterStatComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InInstigater, AActor* Causer)
{
	if (bIsDead) return;
	float FinalDamage = FMath::Min(Damage,Stats.CurrentHP);
	Stats.CurrentHP -= FinalDamage; 
	
	OnHpChanged.Broadcast(Stats.CurrentHP,Stats.MaxHP,FinalDamage);
	
	if (Stats.CurrentHP == 0)
	{
		bIsDead= true;
		OnDeath.Broadcast(InInstigater);
	}
}

float UMonsterStatComponent::GetAttackRange() const
{
	return Stats.AttackRange;
}

float UMonsterStatComponent::GetAttackRate() const
{
	return Stats.AttackRate;
}

bool UMonsterStatComponent::IsDead() const
{
	return bIsDead;
}


