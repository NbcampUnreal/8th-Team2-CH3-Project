// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPlasmaOrb.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/BattleSubsystem.h"
ABossPlasmaOrb::ABossPlasmaOrb()
{
    // 충돌체 세팅
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(70.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->OnComponentHit.AddDynamic(this, &ABossPlasmaOrb::OnHit);
    
    RootComponent = CollisionComp;
    
    // 투사체 이동 컴포넌트 세팅
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 700.0f;
    ProjectileMovement->MaxSpeed = 700.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 없이 직선 비행
}

void ABossPlasmaOrb::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
}

float ABossPlasmaOrb::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    CurrentHealth -= ActualDamage;
    
    if (CurrentHealth <= 0.0f)
    {
        OnReadyToReturn.Broadcast(this);
    }

    return ActualDamage;
}
void ABossPlasmaOrb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 부딪힌 대상이 없거나 나 자신이면 크래시 방지를 위해 리턴
    if (!OtherActor || OtherActor == GetOwner()) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    
    if (PlayerPawn && OtherActor == PlayerPawn)
    {
        UBattleSubsystem* BattleSubsystem = GetWorld()->GetGameInstance() ? GetWorld()->GetGameInstance()->GetSubsystem<UBattleSubsystem>() : nullptr;
        // 데미지 적용
        if (BattleSubsystem)
        {
            BattleSubsystem->ExecuteDamageCalculation(
                GetOwner(),
                PlayerPawn, 
                OrbDamage, 
                false, 
                1
               );
        }
        DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 5, FColor::Green, false, 3.f);
    }
    OnReadyToReturn.Broadcast(this);
}

void ABossPlasmaOrb::OnSpawnFromPool(const FTransform& Transform)
{
    SetActorTransform(Transform);
    
    CurrentHealth = MaxHealth;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    if (ProjectileMovement)
    {
        ProjectileMovement->UpdatedComponent = RootComponent;
        ProjectileMovement->Velocity = Transform.GetRotation().GetForwardVector() * ProjectileMovement->InitialSpeed;
        ProjectileMovement->Activate(true);
    }
}

void ABossPlasmaOrb::OnReturnToPool()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    if (ProjectileMovement)
    {
        ProjectileMovement->Deactivate();
    }
}