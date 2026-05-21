// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicMoonRabbit.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"

void URelicMoonRabbit::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	 GetWorld()->GetTimerManager().SetTimer(
	 	SpawnMoonRabbitEffectTimer,
	 	this,
	 	&URelicMoonRabbit::SpawnMoonRabbitEffect,
	 	15.f,
	 	true,
	 	1.f
	 );
}

void URelicMoonRabbit::SpawnMoonRabbitEffect() const
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	FVector SpawnPoint = Player->GetActorLocation();
	SpawnPoint.Z += 50.f;
	
	GetWorld()->SpawnActor<AActor>(
		MoonRabbitEffectActor,
		SpawnPoint,
		FRotator::ZeroRotator
	); 
}
