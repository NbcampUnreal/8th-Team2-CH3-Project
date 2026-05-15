// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "MonsterBase.h"
#include "PoolComponent.h"
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

	//소환 테스트용
	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::RandomSpawnMonster, 1.0f, true);
	RandomSpawnMonster();
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


AMonsterBase* AMonsterSpawner::SpawnMonster(TSubclassOf<AMonsterBase> MonsterClass, const FTransform& Transform,
                                            const FMonsterStats& InStats)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(PoolComp->GetActorFromPool(MonsterClass, Transform));

	if (Monster)
	{
		Monster->SetMonsterStats(InStats);
		return Monster;
	}

	return nullptr;
}

void AMonsterSpawner::RandomSpawnMonster()
{
	if (MonsterConfigs.IsEmpty())
	{
		return;
	}

	int32 RandomIdx = FMath::RandRange(0, MonsterConfigs.Num() - 1);
	FMonsterSpawnConfig* SelectedConfig = MonsterConfigs[RandomIdx];
	if (!SelectedConfig)return;

	FVector RandomLoc = GetActorLocation() + FVector(FMath::RandRange(-200.f, 200.f), FMath::RandRange(-200.f, 200.f),
	                                                 0.0f);
	FTransform Transform(FRotator::ZeroRotator, RandomLoc, FVector::OneVector);
	SpawnMonster(SelectedConfig->MonsterClass, Transform, SelectedConfig->MonsterStats);
}
