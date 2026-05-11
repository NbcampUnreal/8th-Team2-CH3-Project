// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
FOnReadyToReturn& AMonsterBase::GetOnReadyToReturn()
{
	//몬스터 델리게이트 전달
	return OnMonsterDeath;
}

float AMonsterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	CurrentStats.CurrentHP = FMath::Clamp(CurrentStats.CurrentHP-ActualDamage, 0.f,CurrentStats.MaxHP);
	if (CurrentStats.CurrentHP <= 0.f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void AMonsterBase::OnDeath()
{
	//2초뒤 풀로 돌아감
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle,this,&AMonsterBase::AfterDeath,2.f,false);
}

void AMonsterBase::InitializeStats()
{
	
}

void AMonsterBase::OnSpawnFromPool(const FTransform& Transform)
{
	SetActorLocationAndRotation(Transform.GetLocation(),Transform.GetRotation());
	GetCharacterMovement()->StopMovementImmediately();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	
	CurrentStats=BaseStats;
}

void AMonsterBase::OnReturnToPool()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	GetCharacterMovement()->StopMovementImmediately();
}

void AMonsterBase::AfterDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	if (OnMonsterDeath.IsBound())
	{
		//죽었다고 신호 뿌림
		OnMonsterDeath.Broadcast(this);
	}
	else
	{
		Destroy();
	}
}

