#include "Battle/BattleSubsystem.h"
#include "APlayer.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "MonsterTypes.h"
#include "MonsterBase.h"
#include "MonsterStatComponent.h"
#include "Data/MasterSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UMasterSubsystem>();

	Super::Initialize(Collection);
	
	if (GetGameInstance())
	{
		if (UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>())
		{
			MasterSubsystem->OnGameEnd.AddDynamic(this, &UBattleSubsystem::BroadcastBattleResult);
		}
	}
}

void UBattleSubsystem::Deinitialize()
{
	if (GetGameInstance())
	{
		if (UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>())
		{
			MasterSubsystem->OnGameEnd.RemoveDynamic(this, &UBattleSubsystem::BroadcastBattleResult);
		}
	}
	
	Super::Deinitialize();
}

void UBattleSubsystem::ExecuteDamageCalculation(AActor* Attacker, AActor* Victim, float BaseDamage, bool bIsCritical, float CritMultiplier)
{
	if (!Attacker || !Victim)
	{
		return;
	}
	
	if (!CachedPlayer.IsValid())
	{
		CachedPlayer = Cast<AAPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	float FinalDamage = CalculateFinalDamage(Attacker, BaseDamage, bIsCritical, CritMultiplier);
	
	AAPlayer* Player = CachedPlayer.Get();
	if (Attacker->GetOwner() == Player || Attacker == Player)
	{
		TotalDamage += FMath::RoundToInt32(FinalDamage);
	}

	Victim->TakeDamage(
		FinalDamage,
		FDamageEvent(),
		Attacker->GetInstigatorController(),
		Attacker
	);

	ProcessDeathAndKillCount(Victim);
	
	// Test Log
	UE_LOG(LogTemp, Log, TEXT("=== BATTLE REPORT ==="));
	UE_LOG(LogTemp, Log, TEXT("Attacker: %s | Victim: %s"), *Attacker->GetName(), *Victim->GetName());
	UE_LOG(LogTemp, Log, TEXT("Final Damage: %f | Total Damage: %d"), FinalDamage, TotalDamage);
	UE_LOG(LogTemp, Log, TEXT("====================="));
}

float UBattleSubsystem::CalculateFinalDamage(AActor* Attacker, float BaseDamage, bool bIsCritical, float CritMultiplier)
{
	if (!Attacker || !bIsCritical)
    {
    	return BaseDamage;
    }

	float CalculatedDamage = BaseDamage;
	
	AAPlayer* Player = CachedPlayer.Get();
	if (Attacker->GetOwner() == Player || Attacker == Player)
	{
		CalculatedDamage *= CritMultiplier;
	}
	
	return CalculatedDamage;
}

void UBattleSubsystem::ProcessDeathAndKillCount(AActor* Victim)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(Victim);
	if (!Monster || !Monster->GetStatComponent() || !Monster->GetStatComponent()->IsDead())
	{
		return;
	}

	EMonsterGrade VictimGrade = Monster->GetStatComponent()->GetMonsterTag();
	switch (VictimGrade)
	{
	case EMonsterGrade::Melee:
		{
			++MeleeKills;
			break;
		}
	case EMonsterGrade::Ranged:
		{
			++RangedKills;
			break;
		}
	case EMonsterGrade::EliteMelee:
		{
			++EliteMeleeKills;
			break;
		}
	case EMonsterGrade::EliteRanged:
		{
			++EliteRangedKills;
			break;
		}
	case EMonsterGrade::Boss:
		{
			++BossKills;
			break;
		}
	default:
		{
			UE_LOG(LogTemp, Warning, TEXT("MonsterGrade None"));
			break;
		}
	}
}

void UBattleSubsystem::BroadcastBattleResult()
{
	if (!GetGameInstance())
	{
		return;
	}
	
	if (UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>())
	{
		MasterSubsystem->OnBattleResult.Broadcast(MeleeKills, RangedKills, EliteMeleeKills, EliteRangedKills, BossKills, TotalDamage);
	}
}
