#include "RelicManager.h"
#include "Ch3_Team2/Data/MasterSubsystem.h"

ARelicManager::ARelicManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARelicManager::BeginPlay()
{
    Super::BeginPlay();
    
    if (GetWorld() && ApplyManagerClass)
    {
       ApplyManager = GetWorld()->SpawnActor<ARelicApplyManager>(ApplyManagerClass);
    }
}

void ARelicManager::LoadData(TArray<int32> RelicIDs)
{
    if (!ApplyManager || !IsValid(RelicDataTable)) 
    {
       return;
    }
    
    RandomRelicOption.Empty();
    AllRelics.Empty();
    OwnedRelicIDs.Empty();
    
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
    
    if (RelicIDs.Num() == 0)
    {
       return;
    }
    
    OwnedRelicIDs.Empty();
    
    for (int32 RelicID : RelicIDs)
    {
       OwnedRelicIDs.Add({RelicID, false});
       
       if (RelicID >= 1017)
       {
          BlockedRelicIDs.Add(RelicID);
       }
    }
    
    if (OwnedRelicIDs.Num() != 0)
    {
       ApplyManager->ApplyRelicById(OwnedRelicIDs);
    }
}

void ARelicManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
    if (GetGameInstance())
    {
       MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
       if (MasterSubsystem)
       {
          TArray<int32> PureRelicIDs;
           
          for (const TPair<int32, bool>& Pair : OwnedRelicIDs)
          {
             PureRelicIDs.Add(Pair.Key);
          }
          MasterSubsystem->OnSaveRelic.Broadcast(PureRelicIDs);
       }
    }
    
    Super::EndPlay(EndPlayReason);
}

void ARelicManager::AddOwnedRelic(const FRelicData& NewRelic)
{
    if (NewRelic.RelicId == 0) return;
   
    if (!ApplyManager)
    {
        return;
    }

    
    OwnedRelicIDs.Add({NewRelic.RelicId, false});
    RandomRelicOption.Empty();
    
    if (NewRelic.RelicId >= 1017)
    {
       BlockedRelicIDs.Add(NewRelic.RelicId);
    }
    
    ApplyManager->ApplyRelicById(OwnedRelicIDs);
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
       
       if (!PickedRelic.RelicId)
       {
          return;
       }
       
       bool bIsBlocked = BlockedRelicIDs.Contains(PickedRelic.RelicId);
       if (bAlreadyInOption || bIsBlocked) continue;
       
       RandomRelicOption.Add(PickedRelic);
    }
}

bool ARelicManager::GetRandomRelicByGrade(ERelicGrade Grade, FRelicData& OutRelic)
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
    
    int32 RandIndex = FMath::RandRange(0, Candidates.Num() - 1);
    OutRelic = Candidates[RandIndex];
    return true;
}

void ARelicManager::EliteMonsterDeath()
{
    RandomRelic();
    OnRelicRewardGenerated.Broadcast(RandomRelicOption);
}



