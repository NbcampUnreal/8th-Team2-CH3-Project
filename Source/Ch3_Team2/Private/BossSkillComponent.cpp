// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSkillComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PoolComponent.h"
#include "BossPlasmaOrb.h"
#include "Kismet/KismetMathLibrary.h"
#include "Battle/BattleSubsystem.h"

UBossSkillComponent::UBossSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBossSkillComponent::BeginPlay()
{
	Super::BeginPlay();
    FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), PoolManagerClass);
}

void UBossSkillComponent::PlayPowerSlam()
{
    ACharacter* BossMonster = Cast<ACharacter>(GetOwner());
    if (!BossMonster || !SkillMontage)
    {
        return;
    }

    // 공격 도중 미끄러지지 않게 즉시 이동 차단
    if (BossMonster->GetCharacterMovement())
    {
        BossMonster->GetCharacterMovement()->StopMovementImmediately();
    }

    BossMonster->PlayAnimMontage(SkillMontage);
}

void UBossSkillComponent::ExecutePowerSlamHit()
{
   ACharacter* BossCharacter = Cast<ACharacter>(GetOwner());
       if (!BossCharacter)
       {
           return;
       }
   
       UWorld* World = GetWorld();
       if (!World)
       {
           return;
       }
   
       
       FVector BossLocation = BossCharacter->GetActorLocation();
       FVector ForwardVector = BossCharacter->GetActorForwardVector();
       FVector TraceTargetLocation = BossLocation + (ForwardVector * ForwardOffset);
   
       TArray<FHitResult> HitResults;
       FCollisionShape SphereShape = FCollisionShape::MakeSphere(SlamRadius);
       
       FCollisionQueryParams QueryParams;
       QueryParams.AddIgnoredActor(BossCharacter);
   
       bool bHit = World->SweepMultiByChannel(
           HitResults,
           BossLocation,          
           TraceTargetLocation,   
           FQuat::Identity,      
           ECC_Visibility,       
           SphereShape,          
           QueryParams
       );
    
       if (!bHit)
       {
           return;
       }
   
       // 중복 대미지 방지용 배열
       TArray<AActor*> DamagedActors;
    
        //타겟 검사
       for (const FHitResult& Hit : HitResults)
       {
           AActor* HitActor = Hit.GetActor();
           if (!HitActor || DamagedActors.Contains(HitActor)) continue;
   
           ACharacter* TargetCharacter = Cast<ACharacter>(HitActor);
           APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
           
           if (PlayerPawn && PlayerPawn == TargetCharacter)
           {
               DamagedActors.Add(TargetCharacter);
   
               // 점프 회피 판정
               UCharacterMovementComponent* MoveComp = TargetCharacter->GetCharacterMovement();
               if (MoveComp && MoveComp->IsFalling())
               {
                   GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Player Evaded Power Slam by Jumping!"));
                   continue; 
               }
               
               UBattleSubsystem* BattleSubsystem = GetWorld()->GetGameInstance() ? GetWorld()->GetGameInstance()->GetSubsystem<UBattleSubsystem>() : nullptr;
               // 데미지 적용
               if (BattleSubsystem)
               {
                   if (HitActor == PlayerPawn)
                   {
                       BattleSubsystem->ExecuteDamageCalculation(
               GetOwner(), 
               PlayerPawn, 
                       SlamDamage, 
               false, 
               1
                       );
                   }
               }
           }
       }
}

void UBossSkillComponent::PlayPlasmaOrb()
{
    ACharacter* BossCharacter = Cast<ACharacter>(GetOwner());
    if (!BossCharacter || !PlasmaMontage)
    {
        return;
    }

    if (BossCharacter->GetCharacterMovement())
    {
        BossCharacter->GetCharacterMovement()->StopMovementImmediately();
    }

    BossCharacter->PlayAnimMontage(PlasmaMontage);
}

void UBossSkillComponent::ExecuteSpawnPlasmaOrb()
{
    ACharacter* BossCharacter = Cast<ACharacter>(GetOwner());
    if (!BossCharacter || !PlasmaOrbClass)
    {
        return;
    }
    if (!FoundActor)
    {
        return;
    }
    UPoolComponent* PoolComp = FoundActor->FindComponentByClass<UPoolComponent>();
    if (!PoolComp)
    {
        return;
    }
    
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    if (!PlayerPawn)
    {
        return;
    }
    
    
    FVector SpawnLocation =BossCharacter->GetActorLocation();
    FVector TargetLocation = PlayerPawn->GetActorLocation();
    FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(BossCharacter->GetActorLocation() ,TargetLocation);
    SpawnLocation = SpawnLocation + (BossCharacter->GetActorForwardVector() * 150.0f) + FVector(0.0f, 0.0f, 60.0f);
                          
    FTransform SpawnTransform(SpawnRotation,SpawnLocation);
    
    
    AActor* PooledActor = PoolComp->GetActorFromPool(PlasmaOrbClass, SpawnTransform);
    if (!PooledActor)
    {
        return;
    }
    
    if (ABossPlasmaOrb* Projectile = Cast<ABossPlasmaOrb>(PooledActor))
    {
        // 주인 설정
        Projectile->SetOwner(BossCharacter);
        Projectile->SetInstigator(BossCharacter);

        // 주인 무시 설정
        if (UPrimitiveComponent* ProjCollision = Cast<UPrimitiveComponent>(Projectile->GetRootComponent()))
        {
            ProjCollision->IgnoreActorWhenMoving(BossCharacter, true);
        }
    }
    
}