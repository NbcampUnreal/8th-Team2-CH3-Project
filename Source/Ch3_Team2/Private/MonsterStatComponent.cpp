// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"

UMonsterStatComponent::UMonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this,&UMonsterStatComponent::TakeDamage);
}

void UMonsterStatComponent::InitializeStats(const FMonsterStats& NewStats)
{
	Stats = NewStats;
	Stats.CurrentHP = Stats.MaxHP;
	bIsDead = false;
}

void UMonsterStatComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InInstigater, AActor* Causer)
{
	if (bIsDead) return;
	float FinalDamage = FMath::Min(Damage,Stats.CurrentHP);
	Stats.CurrentHP -= FMath::RoundToInt32(FinalDamage); 
	
	if (Stats.CurrentHP <= 0)
	{
		bIsDead= true;
		OnDeath.Broadcast(InInstigater);
	}
	
	// Test Log
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("HP: %d / Damage: %f"), 
		   Stats.CurrentHP, FinalDamage);
            
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, Message);
	}
	UE_LOG(LogTemp, Log, TEXT("=== Monster REPORT ==="));
	UE_LOG(LogTemp, Log, TEXT("Monster HP: %d"), Stats.CurrentHP);
	UE_LOG(LogTemp, Log, TEXT("Final Damage: %f"), FinalDamage);
	UE_LOG(LogTemp, Log, TEXT("====================="));
}

bool UMonsterStatComponent::IsDead() const
{
	return bIsDead;
}


