#include "TotemSpawner.h"
#include "HealTotem.h"
#include "PoolComponent.h"
#include "NavigationSystem.h"

ATotemSpawner::ATotemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
    
	PoolComp = CreateDefaultSubobject<UPoolComponent>(TEXT("TotemPool"));
}

void ATotemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (TotemClass)
	{
        PreAllocateTotems();
		for (int32 i = 0; i < TargetSpawnCount; ++i)
		{
			RandomSpawnTotem();
		}
     }
}

void ATotemSpawner::OnTotemReturned(AActor* ReturnedActor)
{
	CurrentActiveTotems--;
	
	if (AHealTotem* ReturnedTotem = Cast<AHealTotem>(ReturnedActor))
	{
		ActiveTotemList.Remove(ReturnedTotem);
	}
	
	if (CurrentActiveTotems < TargetSpawnCount)
	{
		RandomSpawnTotem();
	}
}

void ATotemSpawner::PreAllocateTotems()
{
	if (!TotemClass || !PoolComp)
	{
		return;
	}

	for (int32 i = 0; i < PreAllocateCount; ++i)
	{
		AActor* Temp = PoolComp->GetActorFromPool(TotemClass, GetActorTransform());
		PoolComp->ReturnActorToPool(Temp);
	}
}

AHealTotem* ATotemSpawner::SpawnHealTotem(const FTransform& Transform)
{
	if (!TotemClass || !PoolComp)
	{
		return nullptr;
	}

	AHealTotem* HealTotem = Cast<AHealTotem>(PoolComp->GetActorFromPool(TotemClass, Transform));
	if (HealTotem)
	{
		return HealTotem;
	}

	return nullptr;
}

void ATotemSpawner::RandomSpawnTotem()
{
	if (!TotemClass || CurrentActiveTotems >= TargetSpawnCount)
	{
		return;
	}
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return;
	}

	FNavLocation RandomNavLocation;
	if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), SpawnBound, RandomNavLocation))
	{
		bool bTooClose = false;
		for (AHealTotem* ActiveTotem : ActiveTotemList)
		{
			if (ActiveTotem)
			{
				// 새로 뽑은 위치와 이미 배치된 토템들 사이의 거리를 검사
				float Distance = FVector::Dist(RandomNavLocation.Location, ActiveTotem->GetActorLocation());
				if (Distance < MinDistanceBetweenTotems)
				{
					bTooClose = true;
					break; // 하나라도 가까우면 루프 탈출
				}
			}
		}

		// 다른 토템과 겹친다면 이번 소환은 취소하고 다음 틱에 재시도 (ClearTimer 필요 없음)
		if (bTooClose)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATotemSpawner::RandomSpawnTotem);
			return;
		}
		
		FTransform SpawnTransform(FRotator::ZeroRotator, RandomNavLocation.Location, FVector::OneVector);
		AHealTotem* NewTotem = SpawnHealTotem(SpawnTransform);
		if (NewTotem)
		{
			NewTotem->GetOnReadyToReturn().AddUniqueDynamic(this, &ATotemSpawner::OnTotemReturned);
			CurrentActiveTotems++;

			ActiveTotemList.Add(NewTotem);
			return;
		}
	}

	// 실패했다면 다시 시도
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATotemSpawner::RandomSpawnTotem);
}