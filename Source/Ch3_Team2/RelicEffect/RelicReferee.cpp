// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicReferee.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"

void URelicReferee::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	GetWorld()->GetTimerManager().SetTimer(
		RelicRefereeTimerHandle,
		this,
		&URelicReferee::SpawnRefereeEffect,
		15.f,
		true,
		1.f
	);
}

void URelicReferee::SpawnRefereeEffect() const
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	FVector SpawnLocation = Player->GetActorLocation();
	
	GetWorld()->SpawnActor<AActor>(
		RefereeBP,
		SpawnLocation,
		FRotator::ZeroRotator
	);
	
}
