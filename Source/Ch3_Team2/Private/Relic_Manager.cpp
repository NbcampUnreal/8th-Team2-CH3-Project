#include "Relic_Manager.h"


ARelic_Manager::ARelic_Manager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARelic_Manager::AddOwnedRelic(const FRelicData& NewRelic)
{
	OwnedRelics.Add(NewRelic);
}

void ARelic_Manager::BeginPlay()
{
	Super::BeginPlay();

	RandomRelicOption.Empty();
	AllRelics.Empty();

	if (!RelicDataTable)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("RelicDataTable is Null!!!"));
		return;
	}

	TArray<FRelicData*> RelicData;
	RelicDataTable->GetAllRows(TEXT("Relic"), RelicData);

	for (FRelicData* Relic : RelicData)
	{
		if (!Relic) continue;

		AllRelics.Add(*Relic);

		UE_LOG(LogTemp, Warning,
			TEXT("%s"),
			*Relic->RelicName.ToString());
	}

	RandomRelic();

	for (FRelicData Relic : RandomRelicOption)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("%d"),
			Relic.RelicId);
	}
}

ERelicGrade ARelic_Manager::NormalRollGrade()
{
	int32 Rand = FMath::RandRange(1, 100);
	
	if (Rand <= 80) return ERelicGrade::Common;
	if (Rand <= 95) return ERelicGrade::Rare;
	if (Rand <= 99) return ERelicGrade::Epic;
	return ERelicGrade::Legendary;
}

void ARelic_Manager::RandomRelic()
{
	RandomRelicOption.Empty();

	int32 TryCount = 0;

	while (RandomRelicOption.Num() < 3 && TryCount < 100)
	{
		TryCount++;

		ERelicGrade Grade = NormalRollGrade();

		FRelicData PickedRelic;

		if (!GetRandomRelicByGrade(Grade, PickedRelic))
		{
			continue;
		}

		bool bAlreadyInOption =
			RandomRelicOption.ContainsByPredicate([&](const FRelicData& RelicData)
			{
				return RelicData.RelicId == PickedRelic.RelicId;
			});

		if (bAlreadyInOption)
		{
			continue;
		}

		RandomRelicOption.Add(PickedRelic);
	}
}

bool ARelic_Manager::GetRandomRelicByGrade(
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

	if (Candidates.Num() == 0)
	{
		return false;
	}

	int32 RandIndex =
		FMath::RandRange(0, Candidates.Num() - 1);

	OutRelic = Candidates[RandIndex];

	return true;
}
