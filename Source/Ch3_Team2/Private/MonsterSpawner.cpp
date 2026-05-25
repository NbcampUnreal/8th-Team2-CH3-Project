// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "MonsterBase.h"
#include "PoolComponent.h"
#include "Kismet/GameplayStatics.h"  
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"

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
	GetWorld()->GetTimerManager().ClearTimer(BossSpawnTimerHandle);
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
	
	FTransform SpawnTransform;
	GetMonsterSpawnTransform(BossConfig,SpawnTransform);
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FRotator SpawnRotation = (PlayerLocation - SpawnTransform.GetLocation()).Rotation();
	SpawnRotation.Pitch = 0.f; 
	
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	SpawnMonster(BossConfig->MonsterClass, SpawnTransform, BossConfig->MonsterStats);
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
	    return;
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys)
    {
    	return;
    }

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    FNavLocation NavMeshLocation;
    bool bFoundValidLocation = false;
	
	//적절한 소환 위치 찾기
    for (int32 TryCount = 0; TryCount < 10; ++TryCount)
    {
        float MinSpawnRadius = 500.f;  
        float MaxSpawnRadius = 1200.f; 
        
        float RandomRadius = FMath::FRandRange(MinSpawnRadius, MaxSpawnRadius);
        float RandomAngle = FMath::FRandRange(0.f, 2.f * PI);

        FVector SpawnOffset(FMath::Cos(RandomAngle) * RandomRadius, FMath::Sin(RandomAngle) * RandomRadius, 0.f);
        FVector TargetXYLocation = PlayerLocation + SpawnOffset;

        // 지형 고저차 레이캐스트
        FVector TraceStart = TargetXYLocation + FVector(0.f, 0.f, 1000.f); 
        FVector TraceEnd = TargetXYLocation - FVector(0.f, 0.f, 1000.f);   

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(PlayerPawn); 

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);
        FVector GroundLocation = TargetXYLocation;

        if (bHit)
        {
            GroundLocation = HitResult.Location;
        }
        else
        {
            GroundLocation.Z = PlayerLocation.Z;
        }

        // NavMesh 위에 존재하는지 검사
        FVector SearchExtent(300.f, 300.f, 300.f); 
        if (NavSys->ProjectPointToNavigation(GroundLocation, NavMeshLocation, SearchExtent))
        {
            bFoundValidLocation = true;
            break; 
        }
    }
	
    // 주변이 온통 절벽이거나 10번 다 실패했다면
    if (!bFoundValidLocation)
    {
        // 플레이어 바로 옆에서 가장 가까운 네브메시 자리에 소환
        FVector BackupExtent(500.f, 500.f, 500.f);
        if (!NavSys->ProjectPointToNavigation(PlayerLocation, NavMeshLocation, BackupExtent))
        {
            //실패하면 에러를 내는 대신 플레이어 현재 좌표를 강제 대입
            NavMeshLocation.Location = PlayerLocation;
        }
    }

    // 3. 땅에 박힘 방지를 위한 캡슐 높이 연산 (최종 확정된 NavMeshLocation 기준)
    float CapsuleHalfHeight = 88.f;
    if (AMonsterBase* DefaultMonster = SelectedConfig->MonsterClass->GetDefaultObject<AMonsterBase>())
    {
        if (UCapsuleComponent* MonsterCapsule = DefaultMonster->GetCapsuleComponent())
        {
            CapsuleHalfHeight = MonsterCapsule->GetUnscaledCapsuleHalfHeight();
        }
    }

    // 최종 위치 세팅
    FVector FinalSpawnLocation = NavMeshLocation.Location + FVector(0.f, 0.f, CapsuleHalfHeight);
    
    // 정상 세팅은 초록색, 강제 세팅은 주황색 선으로 디버깅 표시
    FColor DebugColor = bFoundValidLocation ? FColor::Green : FColor::Orange;
    DrawDebugLine(GetWorld(), NavMeshLocation.Location, FinalSpawnLocation, DebugColor, false, 3.f, 0, 2.f);

    Transform = FTransform(FRotator::ZeroRotator, FinalSpawnLocation, FVector::OneVector);
}


