// Fill out your copyright notice in the Description page of Project Settings.

#include "RelicApplyManager.h"

#include "Ch3_Team2/APlayer.h"
#include "Kismet/GameplayStatics.h"
#include "RelicData.h"
#include "RelicGrade.h"
#include "Ch3_Team2/Battle/BattleSubsystem.h"
#include "MonsterBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Algo/Count.h"

void ARelicApplyManager::ApplyRelicById(TArray<TPair<int32, bool>> &RelicIDs)
{
    if (RelicIDs.Num() == 0) return;
    
    for (TPair<int32, bool>& RelicID : RelicIDs)
    {
        if (RelicID.Value == true) continue;
       
        if (RelicID.Key <= 1016)
        {
            FindRelicData(RelicID.Key);
            RelicID.Value = true;
            continue;
        }

        switch (RelicID.Key)
        {
        case 1027: Relic1027(); break;
        case 1117: Relic1117(); break;
        case 1118: Relic1118(); break;
        case 1119: Relic1119(); break;
        case 1120: Relic1120(); break;
        case 1121: Relic1121(); break;
        case 1122: Relic1122(RelicIDs); break;
        case 1123: Relic1123(); break;
        case 1124: Relic1124(); break;
        case 1125: Relic1125(); break;
        default:   break;
        }
        
        RelicID.Value = true;
    }
}

void ARelicApplyManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(TimerHandle1027);
        World->GetTimerManager().ClearTimer(TimerHandle1117);
        World->GetTimerManager().ClearTimer(TimerHandle1118);
        World->GetTimerManager().ClearTimer(TimerHandle1120);
        World->GetTimerManager().ClearTimer(TimerHandle1123);
        World->GetTimerManager().ClearTimer(TimerHandle1124);
    }

    Super::EndPlay(EndPlayReason);
}

void ARelicApplyManager::RelieBattleSystem(float Value, AActor* Victim)
{
    AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    
    if (!Player) return;
    
    UBattleSubsystem* BattleSubsystem = GetGameInstance()->GetSubsystem<UBattleSubsystem>();
    if (!BattleSubsystem) return;
    
    BattleSubsystem->ExecuteDamageCalculation(Player, Victim, Value, false, 1.f);
}

void ARelicApplyManager::FindRelicData(int32 RelicID) const
{
    if (!RelicDataTable) return;
    
    const FString Context(TEXT("Relic"));
    
    TArray<FRelicData*> AllRows;
    
    RelicDataTable->GetAllRows<FRelicData>(Context, AllRows);
    
    for (FRelicData* RelicData : AllRows)
    {
        if (RelicData && RelicData->RelicId == RelicID) 
        {
            RelicStatUp(RelicData->Value, RelicData->RelicStat);
            return;
        }
    }
}



void ARelicApplyManager::RelicStatUp(float Value, ERelicStatType StatType) const
{
    AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (!Player) return; 

    switch (StatType)
    {
    case ERelicStatType::MaxHP:
        Player->AddMaxHp(Value);
        break;
    case ERelicStatType::MoveSpeed:
        Player->AddPlayerSpeed(Value);
        break;
    case ERelicStatType::AmmoDamage:
        Player->TotalDamageUpGrade(Value, 0, 0);
        break;
    case ERelicStatType::critical:
        {
            float CurrentValue = Value * 0.01f;
            Player->TotalDamageUpGrade(0, 0, CurrentValue);
            break;
        }
    case ERelicStatType::SkillCooldown:
        Player->DecreaseSkillCoolTime(Value);
        break;
    default:
        break;
    }
}

void ARelicApplyManager::Relic1027()
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &ARelicApplyManager::RelicStatUp, 6.f, ERelicStatType::MaxHP);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle1027, TimerDelegate, 10.f, true, 10.f);
}

void ARelicApplyManager::Relic1117()
{
    TWeakObjectPtr<ARelicApplyManager> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle1117, [WeakThis]()
    {
        if (!WeakThis.IsValid()) return;
        UWorld* World = WeakThis->GetWorld();
        if (!World) return;

        AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (!Player) return;
       
        UBattleSubsystem* BattleSubsystem = World->GetGameInstance()->GetSubsystem<UBattleSubsystem>();
        if (!BattleSubsystem) return;
       
        FVector Center = Player->GetActorLocation();
        float Radius = 700.f;
       
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
       
        TArray<AActor*> IgnoreActors;
        IgnoreActors.Add(Player);
       
        TArray<AActor*> OverlappedActors;
       
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(World,WeakThis->Aurora,Center,FRotator::ZeroRotator,FVector(1.f),true,true);
        
        UKismetSystemLibrary::SphereOverlapActors(
            World, Center, Radius, ObjectTypes, AMonsterBase::StaticClass(), IgnoreActors, OverlappedActors
        );

        for (AActor* Actor : OverlappedActors)
        {
            BattleSubsystem->ExecuteDamageCalculation(Player, Actor, 50.f, false, 1.f);
        }
    }, 5.f, true, 5.f);
}

void ARelicApplyManager::Relic1118()
{
    TWeakObjectPtr<ARelicApplyManager> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle1118, [WeakThis]()
    {
        if (!WeakThis.IsValid()) return;
        UWorld* World = WeakThis->GetWorld();
        if (!World) return;

        AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (!Player) return;
          
        FVector Center = Player->GetActorLocation();
        float SpawnRadius = 1500.f;
        FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
        FVector TargetLocation = Center + FVector(RandomPoint.X, RandomPoint.Y, 0.f);
          
        TargetLocation.Z = 1600.f;
          
        World->SpawnActor<AActor>(WeakThis->ShootingStar, TargetLocation, FRotator::ZeroRotator);
    }, 3.f, true, 3.f);
}

void ARelicApplyManager::Relic1119()
{
    AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (!Player) return;
    
    Player->AddMaxHp(-70);
    Player->TotalDamageUpGrade(0, 50, 0);
}

void ARelicApplyManager::Relic1120()
{
    TWeakObjectPtr<ARelicApplyManager> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle1120, [WeakThis]()
    {
        if (!WeakThis.IsValid()) return;
        UWorld* World = WeakThis->GetWorld();
        if (!World) return;

        AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (!Player) return;
          
        UBattleSubsystem* BattleSubsystem = World->GetGameInstance()->GetSubsystem<UBattleSubsystem>();
        if (!BattleSubsystem) return;
          
        FVector Center = Player->GetActorLocation();
        Center.Z -= 60.f;
          
        float SpawnRadius = 1500.f;
        
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
        TArray<AActor*> IgnoreActors;
        IgnoreActors.Add(Player);

        for (int i = 0; i < 2; ++i)
        {
            FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
            FVector TargetLocation = Center + FVector(RandomPoint.X, RandomPoint.Y, 0.f);
          
            FHitResult HitResult;
            FVector Start = TargetLocation + FVector(0.f, 0.f, 2000.f);
            FVector End = TargetLocation - FVector(0.f, 0.f, 5000.f); 
            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(Player);
          
            if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
            {
                TargetLocation.Z = HitResult.ImpactPoint.Z;
            }
          
            if (WeakThis->LightningFX)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, WeakThis->LightningFX, TargetLocation, FRotator::ZeroRotator);
            }

            TArray<AActor*> OverlappedActors;
            float DamageRadius = 500.f;
            float HalfHeight = 400.f;
          
            UKismetSystemLibrary::CapsuleOverlapActors(
                World, TargetLocation, DamageRadius, HalfHeight, ObjectTypes, AMonsterBase::StaticClass(), IgnoreActors, OverlappedActors
            );
             
            for (AActor* Actor : OverlappedActors)
            {
                BattleSubsystem->ExecuteDamageCalculation(Player, Actor, 20.f, false, 1.f);
            }
        }
    }, 5.f, true, 5.f);
}

void ARelicApplyManager::Relic1121()
{
    AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (!Player) return;
    
    FVector SpawnLocation = Player->GetActorLocation();
    GetWorld()->SpawnActor<AActor>(MoonBook, SpawnLocation, FRotator::ZeroRotator);
}

void ARelicApplyManager::Relic1122(TArray<TPair<int32,bool>> OwnRelicCount)
{
    int32 FalseCount = Algo::CountIf(OwnRelicCount, [](const TPair<int32,bool>& Pair){return Pair.Value == false;});
    RelicStatUp(10 * (FalseCount + 1),ERelicStatType::MaxHP);
    RelicStatUp(1 * (FalseCount + 1),ERelicStatType::AmmoDamage);
}

void ARelicApplyManager::Relic1123()
{
    TWeakObjectPtr<ARelicApplyManager> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle1123, [WeakThis]()
    {
        if (!WeakThis.IsValid()) return;
        UWorld* World = WeakThis->GetWorld();
        if (!World) return;

        AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (!Player) return;
          
        FVector SpawnLocation = Player->GetActorLocation();
        World->SpawnActor<AActor>(WeakThis->Referee, SpawnLocation, FRotator::ZeroRotator);
    }, 15.f, true, 1.f);
}

void ARelicApplyManager::Relic1124()
{
    TWeakObjectPtr<ARelicApplyManager> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle1124, [WeakThis]()
    {
        if (!WeakThis.IsValid()) return;
        UWorld* World = WeakThis->GetWorld();
        if (!World) return;

        AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (!Player) return;
          
        FVector SpawnPoint = Player->GetActorLocation();
        SpawnPoint.Z += 50.f;
          
        World->SpawnActor<AActor>(WeakThis->MoonRabbit, SpawnPoint, FRotator::ZeroRotator);
    }, 15.f, true, 1.f);
}

void ARelicApplyManager::Relic1125()
{
    AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    
    if (!Player) return;
    
    Player->bHasReviveRelic = true;
}

void ARelicApplyManager::Revive()
{
    AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    
    if (!Player) return;
    
        if (Player->bHasReviveRelic)
        {
            Player->AddCurrentHp((Player->GetMapHp() / 10));
			
            FVector PlayerLocation = Player->GetActorLocation();
			
            float Radius = 1300.f;
			
            TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
            ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
			
            TArray<AActor*> IgnoreActors;
            IgnoreActors.Add(Player);
			
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                Diary,
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
                    500.f,
                    Player->GetController(),
                    Player,
                    nullptr
                );
            }
            Player->bHasReviveRelic = false;
        }
    
}


