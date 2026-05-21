#include "RelicManager.h"
#include "Ch3_Team2/RelicEffect/RelicEffectBase.h"
#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"



ARelicManager::ARelicManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARelicManager::AddOwnedRelic(const FRelicData& NewRelic)
{
	if (NewRelic.RelicId == 0) return;
	
	UE_LOG(LogTemp, Warning,TEXT("Relic Selected : %s"), *NewRelic.RelicName.ToString());
	OwnedRelics.Add(NewRelic);
	RandomRelicOption.Empty();
	
	if (NewRelic.Grade == ERelicGrade::Epic || NewRelic.Grade == ERelicGrade::Legendary)
	{
		BlockedRelicIDs.Add(NewRelic.RelicId);
	}
	
	if (!NewRelic.EffectClass) return;
	
	URelicEffectBase* Effect = NewObject<URelicEffectBase>(this, NewRelic.EffectClass);
	AAPlayer* Player =
	Cast<AAPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (Effect)
	{
		Effect->ApplyRelic(Player,NewRelic);
	}
}

void ARelicManager::BeginPlay()
{
	Super::BeginPlay();
	
	RandomRelicOption.Empty();
	AllRelics.Empty();

	if (!RelicDataTable) return;
	
	TArray<FRelicData*> AllRelicRows;
	
	FString ContextString(TEXT("Relic Data Context"));
	
	RelicDataTable->GetAllRows<FRelicData>(ContextString, AllRelicRows);
	for (FRelicData* RelicPtr : AllRelicRows)
	{
		if (!RelicPtr)
		{
		    continue;
		}
		
		AllRelics.Add(*RelicPtr);
	}
}

ERelicGrade ARelicManager::NormalRollGrade()
{
	int32 Rand = FMath::RandRange(1, 100);
	
	if (Rand <= 80) return ERelicGrade::Common;
	if (Rand <= 95) return ERelicGrade::Rare;
	if (Rand <= 99) return ERelicGrade::Epic;
	return ERelicGrade::Legendary;
}

void ARelicManager::RandomRelic()
{
	RandomRelicOption.Empty();

	int32 TryCount = 0;

	while (RandomRelicOption.Num() < 3 && TryCount < 100)
	{
		TryCount++;

		ERelicGrade Grade = NormalRollGrade();

		FRelicData PickedRelic;

		if (!GetRandomRelicByGrade(Grade, PickedRelic)) continue;
		

		bool bAlreadyInOption =
			RandomRelicOption.ContainsByPredicate([&](const FRelicData& RelicData)
			{
				return RelicData.RelicId == PickedRelic.RelicId;
			});
		
		if (!PickedRelic.RelicId) return;
		
		bool bIsBlocked = BlockedRelicIDs.Contains(PickedRelic.RelicId);

		if (bAlreadyInOption || bIsBlocked) continue;
		

		RandomRelicOption.Add(PickedRelic);
	}
}

bool ARelicManager::GetRandomRelicByGrade(
	ERelicGrade Grade,
	FRelicData& OutRelic)
{
	TArray<FRelicData> Candidates;

	for (const FRelicData& Relic : AllRelics)
	{
		if (Relic.Grade == Grade)
		{
			Candidates.Add(Relic);
		}
	}

	if (!Candidates.Num()) return false;
	

	int32 RandIndex =
		FMath::RandRange(0, Candidates.Num() - 1);

	OutRelic = Candidates[RandIndex];

	return true;
}

void ARelicManager::OnEliteMonsterDead()
{
	RandomRelic();
	
	if (OnRelicRewardGenerated.IsBound())
	{
		OnRelicRewardGenerated.Broadcast(RandomRelicOption);
	}
}
