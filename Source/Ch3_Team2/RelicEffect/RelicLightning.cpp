// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicEffect/RelicLightning.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "MonsterBase.h"



void URelicLightning::ApplyRelic(UObject* WorldContextObject, const FRelicData& NewRelic)
{
	GetWorld()->GetTimerManager().SetTimer(
		LightningTimerHandle,
		this,
		&URelicLightning::SpawnLightning,
		1.f,
		true,
		1.f
	);
}

void URelicLightning::SpawnLightning()
{
	for (int i = 0; i < 2; ++i)
	{
		AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
		FVector Center = Player->GetActorLocation();
		Center.Z -= 90.f;
	
		//플레이어 주변 랜덤 위치
		float SpawnRadius = 1500.f;
		FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
		FVector TargetLocation = Center + FVector(RandomPoint.X, RandomPoint.Y, 0.f);
	
		//바닥 체크용 
		FHitResult HitResult;
		FVector Start = TargetLocation + FVector(0.f,0.f,3000.f);
		FVector End = TargetLocation - FVector(0.f,0.f,-3000.f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Player);
	
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start,End,ECC_Visibility, CollisionParams))
		{
			TargetLocation.Z = HitResult.ImpactPoint.Z;
		}
	
		if (LightningFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				LightningFX,
				TargetLocation,
				FRotator::ZeroRotator
			);
		}
		
		TArray<AActor*> OverlapActors;
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(Player);
		
		float DamageRadius = 500.f;
		//지형에 따라 범위 달라지는 것 방지
		float HalfHeight = 400.f;
		
		UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(),
		TargetLocation,
		DamageRadius,
		HalfHeight,
		ObjectTypes,
		AMonsterBase::StaticClass(),
		IgnoreActors,
		OverlapActors
		);
	
		for (AActor* Actor : OverlapActors)
		{
			AMonsterBase* Monster = Cast<AMonsterBase>(Actor);
		
			if (!Monster) continue;
		
			UGameplayStatics::ApplyDamage(
				Monster,
				75.f,
				Player->GetController(),
				Player,
				nullptr
			);
		}
	}
}
