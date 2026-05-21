// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicNearDamage.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterBase.h"

void URelicNearDamage::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	GetWorld()->GetTimerManager().SetTimer(
		GetDamageNearByEnemiesTimer,
		this,
		&URelicNearDamage::DamageNearByEnemies,
		5.f,
		true,
		5.f
		);
}

void URelicNearDamage::DamageNearByEnemies() const
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player) return;
	//범위에 들어온 적들 배열
	TArray<AActor*> OverlappedActors;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	FVector Center = Player->GetActorLocation();
	
	//무시할 액터 배열
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Player);
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		Radius,
		ObjectTypes,
		AMonsterBase::StaticClass(),
		IgnoreActors,
		OverlappedActors
		);
	
	for (AActor* Actor : OverlappedActors)
	{
		AMonsterBase* Monster = Cast<AMonsterBase>(Actor);
		
		if (!Monster) continue;
		
		UGameplayStatics::ApplyDamage(
			Monster,
			50.f,
			Player->GetController(),
			Player,
			nullptr
		);
	}
	
}
