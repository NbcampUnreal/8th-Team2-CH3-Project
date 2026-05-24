// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "MonsterBase.h"
#include "PoolComponent.h"
#include "Kismet/GameplayStatics.h"  
#include "Components/CapsuleComponent.h"
// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PoolComp = CreateDefaultSubobject<UPoolComponent>(TEXT("MonsterPool"));
}

void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (MonsterTable)
	{
		//데이터 테이블에서 몬스터 정보 가져오고 미리 생성
		FString ContextString(TEXT("MonsterSpawner"));
		MonsterTable->GetAllRows<FMonsterSpawnConfig>(ContextString, MonsterConfigs);
		PreAllocateMonsters();
	}


	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::RandomSpawnMonster, 3.0f, true);
	GetWorld()->GetTimerManager().SetTimer(BossSpawnTimerHandle, this, &AMonsterSpawner::SpawnBossMonster, BossSpawnDelay, false);
}

void AMonsterSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}


void AMonsterSpawner::PreAllocateMonsters()
{
	if (MonsterConfigs.IsEmpty())
	{
		return;
	}

	for (FMonsterSpawnConfig* Config : MonsterConfigs)
	{
		if (!Config || !Config->MonsterClass) continue;

		for (int32 i = 0; i < Config->PreAllocateCount; ++i)
		{
			AActor* Temp = PoolComp->GetActorFromPool(Config->MonsterClass, GetActorTransform());
			PoolComp->ReturnActorToPool(Temp);
		}
	}
}


AMonsterBase* AMonsterSpawner::SpawnMonster(TSubclassOf<AMonsterBase> MonsterClass, const FTransform& Transform, const FMonsterStats& InStats)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(PoolComp->GetActorFromPool(MonsterClass, Transform));

	if (Monster)
	{
		Monster->SetMonsterStats(InStats);
		
		//몬스터 죽었을 때 카운트 감소용
		Monster->GetOnReadyToReturn().AddUniqueDynamic(this, &AMonsterSpawner::OnMonsterDespawned);
		CurrentActiveMonsters++;
		
		return Monster;
	}

	return nullptr;
}

void AMonsterSpawner::RandomSpawnMonster()
{
	if (CurrentActiveMonsters >= MaxActiveMonsters)
	{
		return;
	}
	
    if (MonsterConfigs.IsEmpty())
    {
	    return;
    }
	
	FMonsterSpawnConfig* SelectedConfig = GetRandomMonster();
	
	if (!SelectedConfig || !SelectedConfig->MonsterClass)
	{
		return;
	}
	
	FTransform SpawnTransform;
	GetMonsterSpawnTransform(SelectedConfig,SpawnTransform);
	
    SpawnMonster(SelectedConfig->MonsterClass, SpawnTransform, SelectedConfig->MonsterStats);
}

void AMonsterSpawner::OnMonsterDespawned(AActor* DespawnedActor)
{
	
	CurrentActiveMonsters = FMath::Max(0, CurrentActiveMonsters - 1);
}

void AMonsterSpawner::SpawnBossMonster()
{
	if (MonsterConfigs.IsEmpty())
	{
		return;
	}
	FMonsterSpawnConfig* BossConfig = MonsterConfigs[MonsterConfigs.Num()-1];
	
	if (!BossConfig)
	{
		return;
	}
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	
	FTransform SpawnTransform;
	GetMonsterSpawnTransform(BossConfig,SpawnTransform);
	
	// 플레이어를 바라보도록 회전값 계산
	FRotator SpawnRotation = (PlayerLocation - SpawnTransform.GetLocation()).Rotation();
	SpawnRotation.Pitch = 0.f; 
	
	SpawnMonster(BossConfig->MonsterClass, SpawnTransform, BossConfig->MonsterStats);
	GetWorld()->GetTimerManager().ClearTimer(BossSpawnTimerHandle);
}

FMonsterSpawnConfig* AMonsterSpawner::GetRandomMonster()
{
	//누적 확률값 방식
	int32 TotalWeight = 0;
	for (FMonsterSpawnConfig* Config : MonsterConfigs)
	{
		if (Config) TotalWeight += Config->SpawnWeight;
	}

	if (TotalWeight <= 0)
	{
		return nullptr;
	}

	int32 Roll = FMath::RandRange(0, TotalWeight - 1);
	FMonsterSpawnConfig* SelectedConfig = nullptr;

	// 구간 검사로 어떤 몬스터를 스폰할지 결정
	int32 WeightAccumulator = 0;
	for (FMonsterSpawnConfig* Config : MonsterConfigs)
	{
		if (!Config)
		{
			continue;
		}
        
		WeightAccumulator += Config->SpawnWeight;
		if (Roll < WeightAccumulator)
		{
			SelectedConfig = Config;
			break;
		}
	}
	return SelectedConfig;
}

void AMonsterSpawner::GetMonsterSpawnTransform(FMonsterSpawnConfig* SelectedConfig,FTransform& Transform)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return ;
	}
	//플레이어 주변의 랜덤 XY 평면 좌표 계산
    float MinSpawnRadius = 500.f;  
    float MaxSpawnRadius = 1200.f; 
    
    float RandomRadius = FMath::FRandRange(MinSpawnRadius, MaxSpawnRadius);
    float RandomAngle = FMath::FRandRange(0.f, 2.f * PI);

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    FVector SpawnOffset(FMath::Cos(RandomAngle) * RandomRadius, FMath::Sin(RandomAngle) * RandomRadius, 0.f);
    FVector TargetXYLocation = PlayerLocation + SpawnOffset;

    // 지형 고저차 극복을 위한 수직 레이캐스트
    FVector TraceStart = TargetXYLocation + FVector(0.f, 0.f, 1000.f); // 하늘 위로 10m 시점
    FVector TraceEnd = TargetXYLocation - FVector(0.f, 0.f, 1000.f);   // 바닥 아래로 10m 시점

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerPawn); 

    // WorldStatic 채널을 검사하여 땅표면 찾기
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);
    FVector FinalSpawnLocation = TargetXYLocation;

    if (bHit)
    {
        //땅에 박힘 방지
        float CapsuleHalfHeight = 88.f;
        
        if (AMonsterBase* DefaultMonster = SelectedConfig->MonsterClass->GetDefaultObject<AMonsterBase>())
        {
            if (UCapsuleComponent* MonsterCapsule = DefaultMonster->GetCapsuleComponent())
            {
                CapsuleHalfHeight = MonsterCapsule->GetUnscaledCapsuleHalfHeight();
            }
        }

        // 정확한 바닥 위치 + 몬스터 반높이 = 발을 땅에 딛는 위치
        FinalSpawnLocation = HitResult.Location + FVector(0.f, 0.f, CapsuleHalfHeight);
        
        // 디버깅용
        DrawDebugLine(GetWorld(), HitResult.Location, FinalSpawnLocation, FColor::Green, false, 3.f, 0, 2.f);
    }
    else
    {
        // 만약 절벽 바깥이나 공중에 레이 트레이싱이 실패했다면 안전하게 플레이어 높이와 맞추기
        FinalSpawnLocation.Z = PlayerLocation.Z;
    }
	
	FTransform SpawnTransform(FRotator::ZeroRotator, FinalSpawnLocation, FVector::OneVector);
	Transform = SpawnTransform;
}


