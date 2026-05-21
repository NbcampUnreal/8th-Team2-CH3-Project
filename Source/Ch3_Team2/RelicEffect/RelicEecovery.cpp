// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicEecovery.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"

void URelicEecovery::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	GetWorld()->GetTimerManager().SetTimer(
		EcoveryTimer,
		this,
		&URelicEecovery::Healing,
		10.f,
		true,
		10.f
		);
	
	
}

void URelicEecovery::Healing()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	Player->AddCurrentHp(5);
}
