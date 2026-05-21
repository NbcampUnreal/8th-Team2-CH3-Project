// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "MonsterAttackComponent.h"
#include "MonsterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	StatComp = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("StatComp"));
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	if (StatComp)
	{
		StatComp->OnDeath.AddUniqueDynamic(this,&AMonsterBase::HandleDeath);
	}
	
}

FOnReadyToReturn& AMonsterBase::GetOnReadyToReturn()
{
	return OnMonsterDeath;
}

void AMonsterBase::HandleDeath(AController* InInstigator,AActor* DeathActor)
{
	//충돌 및 움직임 중단
	SetActorEnableCollision(false);
	GetCharacterMovement()->StopMovementImmediately();
	
	//2초뒤 풀로 돌아감
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle,this,&AMonsterBase::AfterDeath,2.f,false);
}



void AMonsterBase::SetMonsterStats(const FMonsterStats& InStats)
{
	StatComp->InitializeStats(InStats);
}

void AMonsterBase::OnSpawnFromPool(const FTransform& Transform)
{
	
	SetActorLocationAndRotation(Transform.GetLocation(),Transform.GetRotation());
	GetCharacterMovement()->StopMovementImmediately();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	
	//State tree 활성화
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UStateTreeAIComponent* STComp = AIController->FindComponentByClass<UStateTreeAIComponent>())
		{
			STComp->Activate(true);
			STComp->StartLogic();
		}
	}
}

void AMonsterBase::OnReturnToPool()
{
	//State tree 비활성화
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UStateTreeAIComponent* STComp = AIController->FindComponentByClass<UStateTreeAIComponent>())
		{
			
			STComp->StopLogic(TEXT("Aborted"));
			STComp->Deactivate();
		}
	}
	
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
		OnMonsterDeath.Broadcast(this);
	}
	else
	{
		Destroy();
	}
}



