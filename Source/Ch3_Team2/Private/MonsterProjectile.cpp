// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/BattleSubsystem.h"
#include "DrawDebugHelpers.h"

AMonsterProjectile::AMonsterProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	// 충돌 이벤트 연결
	CollisionComp->OnComponentHit.AddDynamic(this, &AMonsterProjectile::OnHit);
	
	RootComponent = CollisionComp;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f; // 발사 속도
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true; // 날아가는 방향으로 회전
	InitialLifeSpan = 3.0f;
}

void AMonsterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
	{
		return;
	}
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	UBattleSubsystem* BattleSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UBattleSubsystem>() : nullptr;
	if (!PlayerPawn || !BattleSubsystem)
	{
		return;
	}
	if (OtherActor == PlayerPawn)
	{
		BattleSubsystem->ExecuteDamageCalculation(
		GetOwner(), 
		PlayerPawn, 
		Damage, 
		false, 
		1
		);
		DrawDebugSphere(GetWorld(),Hit.ImpactPoint,10.f,5,FColor::Green,false,3.f);
	}
	OnReadyToReturn.Broadcast(this);
}

void AMonsterProjectile::OnSpawnFromPool(const FTransform& Transform)
{
	SetActorTransform(Transform);
    
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
    
	if (ProjectileMovement)
	{
		ProjectileMovement->UpdatedComponent = RootComponent;
		ProjectileMovement->Velocity = Transform.GetRotation().GetForwardVector() * ProjectileMovement->InitialSpeed;
		ProjectileMovement->ProjectileGravityScale=0.f;
		ProjectileMovement->Activate(true);
	}
}

void AMonsterProjectile::OnReturnToPool()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
    
	if (ProjectileMovement)
	{
		ProjectileMovement->Deactivate();
	}
}



