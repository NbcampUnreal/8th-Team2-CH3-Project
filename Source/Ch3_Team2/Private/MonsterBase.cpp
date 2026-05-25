// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"

#include "AGameState.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "MonsterAttackComponent.h"
#include "MonsterStatComponent.h"
#include "Data/MasterSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Data/LevelFlowSubsystem.h"
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

void AMonsterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWorld* World = GetWorld();
	if (World)
	{
		GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

FOnReadyToReturn& AMonsterBase::GetOnReadyToReturn()
{
	return OnMonsterDeath;
}

void AMonsterBase::PlayDeathAnim()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
}

void AMonsterBase::DropExpItem()
{
	UWorld* World = GetWorld();
	if (World && ExpItemClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
        
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		//경험치 아이템을 소환
		World->SpawnActor<AActor>(ExpItemClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AMonsterBase::HandleDeath(AController* InInstigator,AActor* DeathActor)
{
	if (DeathTimerHandle.IsValid())
	{
		return;
	}
	//충돌 및 움직임 중단
	SetActorEnableCollision(false);
	GetCharacterMovement()->StopMovementImmediately();
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UStateTreeAIComponent* STComp = AIController->FindComponentByClass<UStateTreeAIComponent>())
		{
			
			STComp->StopLogic(TEXT("Aborted"));
			STComp->Deactivate();
		}
	}
	
	PlayDeathAnim();
	//2초뒤 풀로 돌아감
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle,this,&AMonsterBase::AfterDeath,2.f,false);
}

void AMonsterBase::SetMonsterStats(const FMonsterStats& InStats)
{
	StatComp->SetDead(false);
	StatComp->InitializeStats(InStats);
	
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

void AMonsterBase::OnSpawnFromPool(const FTransform& Transform)
{
	SetActorLocationAndRotation(Transform.GetLocation(),Transform.GetRotation());
	GetCharacterMovement()->StopMovementImmediately();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void AMonsterBase::OnReturnToPool()
{
	GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	GetCharacterMovement()->StopMovementImmediately();
	
	//State tree 비활성화
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UStateTreeAIComponent* STComp = AIController->FindComponentByClass<UStateTreeAIComponent>())
		{
			
			STComp->StopLogic(TEXT("Aborted"));
			STComp->Deactivate();
		}
	}
}

void AMonsterBase::AfterDeath()
{
	// 경험치 아이템 드롭
	DropExpItem();
	if (StatComp && StatComp->GetMonsterTag()== EMonsterGrade::Boss)
	{
		if (ULevelFlowSubsystem* LevelFlowSubsystem = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>())
		{
			if (LevelFlowSubsystem->GetCurrentLevelIndex() == 3)
			{
				if (UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>())
				{
					MasterSubsystem->OnGameEnd.Broadcast();
				}
			}
			else
			{
				SpawnBossPortal();
			}
		}
	}
	
	
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

void AMonsterBase::SpawnBossPortal()
{
	UWorld* World = GetWorld();
	if (World && PortalClass)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0.f,0.f,100.f);
        
		FRotator SpawnRotation = GetActorRotation();
        
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		AActor* Portal = World->SpawnActor<AActor>(PortalClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (!Portal)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn boss portal at %s"), *SpawnLocation.ToString());
		}
	}
}

void AMonsterBase::BossAfterDeadth()
{
	if (ULevelFlowSubsystem* LevelFlowSubsystem = GetGameInstance()->GetSubsystem<ULevelFlowSubsystem>())
	{
		if (LevelFlowSubsystem->GetCurrentLevelIndex() == 3)
		{
			if (AAGameState* GS = GetWorld()->GetGameState<AAGameState>())
			{
				GS->OnStageEnd.Broadcast();
			}
		}
	}
}
