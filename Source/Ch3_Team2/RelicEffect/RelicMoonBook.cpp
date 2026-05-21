// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicMoonBook.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

void URelicMoonBook::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	SpawnRelic();
	
	
	
	FTimerDelegate Delegate;

	Delegate.BindWeakLambda(this, [this]()
	{
		int32 RandomInt = FMath::RandRange(0, 2);

		switch (RandomInt)
		{
		case 0:
			Heal();
			break;

		case 1:
			SpeedUp();
			break;

		case 2:
			AmmoDamageUp();
			break;
		}
	});

	GetWorld()->GetTimerManager().SetTimer(
		MainTimer,
		Delegate,
		5.f,
		true
	);
}

void URelicMoonBook::BeginDestroy()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(MainTimer);
	}
	
	Super::BeginDestroy();
}

void URelicMoonBook::SpawnRelic() const
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	FVector SpawnLocation = Player->GetActorLocation();
	
	GetWorld()->SpawnActor<AActor>(
		MoonBookBP,
		SpawnLocation,
		FRotator::ZeroRotator
	);
	
	
}

void URelicMoonBook::Heal()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	SpawnRelicEffect(0);
	
	Player->AddCurrentHp(40);
}

void URelicMoonBook::SpeedUp()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!Player) return;
	
	Player->MoveSpeed += 100;
	
	SpawnRelicEffect(1);
	
	GetWorld()->GetTimerManager().SetTimer(
		SpeedUpTimer,
		this,
		&URelicMoonBook::SpeedDown,
		5.f,
		false
		);
}

void URelicMoonBook::SpeedDown()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!Player) return;
	
	Player->MoveSpeed = -100;
	
	GetWorld()->GetTimerManager().ClearTimer(SpeedUpTimer);
}

void URelicMoonBook::AmmoDamageUp()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!Player) return;
	
	Player->TotalDamageUpGrade(25.f,0,0);
	
	SpawnRelicEffect(2);
	
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimer,
		this,
		&URelicMoonBook::AmmoDamageDown,
		5.f,
		false
	);
}

void URelicMoonBook::AmmoDamageDown()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!Player) return;
	
	Player->TotalDamageUpGrade(-25.f,0,0);
	
	GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
}

void URelicMoonBook::SpawnRelicEffect(int a) const
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!Player) return;
	
	FVector SpawnLocation = Player->GetActorLocation();
	
	UNiagaraSystem* SelectedEffect = nullptr;
	
	if (a == 0) SelectedEffect = RelicNiagaraSystemHeal;
	if (a == 1) SelectedEffect = RelicNiagaraSystemSpeedUp;
	if (a == 2) SelectedEffect = RelicNiagaraSystemDamage;
	
	if (!SelectedEffect) return;
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SelectedEffect,
		SpawnLocation,
		FRotator::ZeroRotator,
		FVector(1.f),
		true,
		true
	);
}
