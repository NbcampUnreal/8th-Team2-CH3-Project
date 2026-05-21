// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicDiary.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MonsterBase.h"

void URelicDiary::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	bIsSelected = true;
}

bool URelicDiary::Revive()
{
	if (bIsSelected)
	{
		AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		
		if (Player)
		{
			Player->AddCurrentHp(1000);
			
			FVector PlayerLocation = Player->GetActorLocation();
			
			float Radius = 1300.f;
			
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
			
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(Player);
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				RelicEffect,
				PlayerLocation,
				FRotator::ZeroRotator,
				FVector(2.f),
				true,
				true,
				ENCPoolMethod::AutoRelease,
				true
			);
			
			TArray<AActor*> OverlappedActors;
			
			UKismetSystemLibrary::SphereOverlapActors(
				GetWorld(),
				PlayerLocation,
				Radius,
				ObjectTypes,
				AActor::StaticClass(),
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
			
			bIsSelected = false;
			
			return true;
		}
	}
	else
	{
		return false;
	}
	return false;
}
