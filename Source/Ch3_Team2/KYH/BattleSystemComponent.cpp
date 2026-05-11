// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/BattleSystemComponent.h"
#include "Engine/DamageEvents.h"

UBattleSystemComponent::UBattleSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBattleSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}


//void UBattleSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}


void UBattleSystemComponent::ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier)
{
	if (!Attacker || !Victim) return;
	
	float FinalDamage = BaseDamage;
    
    if (bIsCritical && Attacker->ActorHasTag(FName("Player")))
    {
       FinalDamage = BaseDamage * CritMultiplier;
    }
	
	Victim->TakeDamage(FinalDamage, FDamageEvent(), Attacker->GetInstigatorController(), Attacker);
	
	if (OnBattleResult.IsBound())
    {
        OnBattleResult.Broadcast(Attacker, Victim, FinalDamage, bIsCritical);
    }

	// DamageLog
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("[%s] -> [%s] : %.f Damage!"), 
			*Attacker->GetName(), *Victim->GetName(), FinalDamage);
	        
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, Message);
	}
	UE_LOG(LogTemp, Log, TEXT("=== BATTLE REPORT ==="));
	UE_LOG(LogTemp, Log, TEXT("Attacker: %s"), *Attacker->GetName());
	UE_LOG(LogTemp, Log, TEXT("Victim: %s"), *Victim->GetName());
	UE_LOG(LogTemp, Log, TEXT("Final Damage: %.2f"), FinalDamage);
	UE_LOG(LogTemp, Log, TEXT("====================="));
}