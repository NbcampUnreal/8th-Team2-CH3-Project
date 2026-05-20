// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicProbability.h"
#include "Kismet/GameplayStatics.h"
#include "Ch3_Team2/APlayer.h"
#include "RelicData.h"

void URelicProbability::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	if (NewRelic.RelicId == 1016) bIsGainedDamageRelic = true;
	if (NewRelic.RelicId == 1017) bIsGainedHPRelic = true;
	
	
}

void URelicProbability::DeathMonster()
{
	AddStatProbability();
}

void URelicProbability::AddStatProbability() const
{
	UE_LOG(LogTemp, Log, TEXT("AddStatProbability"));
	if (bIsGainedDamageRelic || bIsGainedHPRelic)
	{
		AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
		if (!Player) return;
	
		if (bIsGainedDamageRelic)
		{
			int32 RandomDamage = FMath::RandRange(1,100);
		
			if (RandomDamage <= 1)
			{
				Player->TotalDamageUpGrade(2.f,0,0);
			}
		}
		if (bIsGainedHPRelic)
		{
			int32 RandomHP = FMath::RandRange(1,100);
		
			if (RandomHP <= 4)
			{
				Player->AddMaxHp(4);
			}
		}
	}
}
