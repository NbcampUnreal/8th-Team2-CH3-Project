// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicShootingStar.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"

void URelicShootingStar::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimer,
		this,
		&URelicShootingStar::SpawnRelicBP,
		3.f,
		true,
		3.f
	);
}

void URelicShootingStar::BeginDestroy()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
	
	Super::BeginDestroy();
}

void URelicShootingStar::SpawnRelicBP()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	FVector Center = Player->GetActorLocation();
	
	float SpawnRadius = 1500.f;
	FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
	FVector TargetLocation = Center + FVector(RandomPoint.X, RandomPoint.Y, 0.f);
	
	TargetLocation.Z = 1500.f;
	
	GetWorld()->SpawnActor<AActor>(
		RelicEffectBP,
		TargetLocation,
		FRotator::ZeroRotator
	);
}

