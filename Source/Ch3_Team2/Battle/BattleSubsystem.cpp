#include "Battle/BattleSubsystem.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "MonsterBase.h"
#include "MonsterStatComponent.h"

void UBattleSubsystem::ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier)
{
	if (!Attacker || !Victim)
	{
		return;
	}

	float FinalDamage = CalculateFinalDamage(Attacker, BaseDamage, bIsCritical, CritMultiplier);
	TotalDamage += FMath::RoundToInt32(FinalDamage);

	Victim->TakeDamage(
		FinalDamage,
		FDamageEvent(),
		Attacker->GetInstigatorController(),
		Attacker
	);

	ProcessDeathAndKillCount(Victim);
	
	// Test Log
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("[%s] -> [%s] : %f Damage!"), 
		   *Attacker->GetName(), *Victim->GetName(), FinalDamage);
            
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, Message);
	}
	UE_LOG(LogTemp, Log, TEXT("=== BATTLE REPORT ==="));
	UE_LOG(LogTemp, Log, TEXT("Attacker: %s | Victim: %s"), *Attacker->GetName(), *Victim->GetName());
	UE_LOG(LogTemp, Log, TEXT("Final Damage: %f | Total Accum: %d"), FinalDamage, TotalDamage);
	UE_LOG(LogTemp, Log, TEXT("====================="));
}

float UBattleSubsystem::CalculateFinalDamage(AActor* Attacker, float BaseDamage, bool bIsCritical, float CritMultiplier)
{
	float CalculatedDamage = BaseDamage;
	if (bIsCritical && Attacker->ActorHasTag(FName("Player")))
	{
		CalculatedDamage *= CritMultiplier;
	}
	
	return CalculatedDamage;
}

void UBattleSubsystem::ProcessDeathAndKillCount(AActor* Victim)
{
    // TODO: 몬스터 델리게이트로 받기
	AMonsterBase* Monster = Cast<AMonsterBase>(Victim);
	if (!Monster || !Monster->GetStatComponent() || !Monster->GetStatComponent()->IsDead())
	{
		return;
	}

	EMonsterGrade VictimGrade = Monster->GetStatComponent()->GetMonsterTag();
	
	bool bFound = false;
	for (FMonsterKillReport& Report : Reports)
	{
		if (Report.MonsterGrade == VictimGrade)
		{
			++Report.KillCount;
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		Reports.Add(FMonsterKillReport(VictimGrade, 1));
	}
}

void UBattleSubsystem::BroadcastBattleResult()
{
	if (!OnBattleResult.IsBound())
	{
		return;
	}
	OnBattleResult.Broadcast(Reports, TotalDamage);
}
