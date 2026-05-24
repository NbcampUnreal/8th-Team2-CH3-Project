// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAttackComponent.h"
#include "MonsterBase.h"
#include "MonsterProjectile.h"
#include "MonsterStatComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Kismet/GameplayStatics.h"   
#include "GameFramework/Character.h"
#include "PoolComponent.h"
void URangedAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	//풀관리 액터 미리 저장
	FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), PoolManagerClass);
	if (!FoundActor)
	{
		return;
	}
}

void URangedAttackComponent::ExecuteAttack()
{
	
	AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner());
	if (Monster && AttackMontage)
	{
		Monster->PlayAnimMontage(AttackMontage);
	}
}

void URangedAttackComponent::CheckHit()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter || !ProjectileClass|| !CachedStatComp)
	{
		return;
	}
	
	// 풀 컴포넌트 찾아오기
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
	
	//플레이어 조준
	FVector MuzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation(TEXT("weapon_r_muzzle"));
	FVector TargetLocation = PlayerPawn->GetActorLocation();
	
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation,TargetLocation);
	FTransform SpawnTransform(SpawnRotation, MuzzleLocation);
	//액터 가져오기
	AActor* PooledActor = PoolComp->GetActorFromPool(ProjectileClass, SpawnTransform);
	if (!PooledActor)
	{
		return;
	}
	
	
	if (AMonsterProjectile* Projectile = Cast<AMonsterProjectile>(PooledActor))
	{
		// 주인 설정 및 데미지 전달
		Projectile->SetOwner(OwnerCharacter);
		Projectile->SetInstigator(OwnerCharacter);
		Projectile->Damage = CachedStatComp->GetAttackDamage();
		
		// 주인 무시 설정
		if (UPrimitiveComponent* ProjCollision = Cast<UPrimitiveComponent>(Projectile->GetRootComponent()))
		{
			ProjCollision->IgnoreActorWhenMoving(OwnerCharacter, true);
		}
	}
	
}


