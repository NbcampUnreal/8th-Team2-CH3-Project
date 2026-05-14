#include "RelicManager.h"
#include "RelicEffectBase.h"
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
		Effect->ApplyEffect(Player,NewRelic);
	}
}

void ARelicManager::BeginPlay()
{
	Super::BeginPlay();

	//테스트 키 바인딩
	APlayerController* PC =
		GetWorld()->GetFirstPlayerController();

	if (!PC) return;

	EnableInput(PC);

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PC->GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(IMC_Relic, 0);
	}

	UEnhancedInputComponent* EIC =
		Cast<UEnhancedInputComponent>(InputComponent);

	if (EIC)
	{
		EIC->BindAction(
			IA_Relic1,
			ETriggerEvent::Started,
			this,
			&ARelicManager::SelectRelic1);

		EIC->BindAction(
			IA_Relic2,
			ETriggerEvent::Started,
			this,
			&ARelicManager::SelectRelic2);

		EIC->BindAction(
			IA_Relic3,
			ETriggerEvent::Started,
			this,
			&ARelicManager::SelectRelic3);
	}
	
	
	RandomRelicOption.Empty();
	AllRelics.Empty();

	if (!RelicDataTable)
	{
		return;
	}
	
	

	TArray<FRelicData*> RelicData;
	RelicDataTable->GetAllRows(TEXT("Relic"), RelicData);

	for (FRelicData* Relic : RelicData)
	{
		if (!Relic) continue;

		AllRelics.Add(*Relic);
	}
	
	RandomRelic();
	
	for (FRelicData Relic : RandomRelicOption)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			FString::Printf(TEXT("Relic : %s"), *Relic.RelicName.ToString())
			);
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

		if (!GetRandomRelicByGrade(Grade, PickedRelic))
		{
			continue;
		}

		bool bAlreadyInOption =
			RandomRelicOption.ContainsByPredicate([&](const FRelicData& RelicData)
			{
				return RelicData.RelicId == PickedRelic.RelicId;
			});
		
		if (PickedRelic.RelicId == 0) return;
		
		bool bIsBlocked = BlockedRelicIDs.Contains(PickedRelic.RelicId);

		if (bAlreadyInOption || bIsBlocked)
		{
			continue;
		}

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

	if (Candidates.Num() == 0)
	{
		return false;
	}

	int32 RandIndex =
		FMath::RandRange(0, Candidates.Num() - 1);

	OutRelic = Candidates[RandIndex];

	return true;
}

void ARelicManager::SelectRelic1()
{
	if (RandomRelicOption.Num() <= 0) return;
	AddOwnedRelic(RandomRelicOption[0]);
}

void ARelicManager::SelectRelic2()
{
	if (RandomRelicOption.Num() <= 0) return;
	AddOwnedRelic(RandomRelicOption[1]);
}

void ARelicManager::SelectRelic3()
{
	if (RandomRelicOption.Num() <= 0) return;
	AddOwnedRelic(RandomRelicOption[2]);
}
