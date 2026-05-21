// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicDoubleEdge.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"

void URelicDoubleEdge::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	Player->AddMaxHp(-70);
	
	Player->TotalDamageUpGrade(0,50,0);
}

