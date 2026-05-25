// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackComponent.h"
#include "MonsterBase.h"
#include "MonsterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"
#include "Engine/GameInstance.h"
#include "Battle/BattleSubsystem.h"
void UMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMeleeAttackComponent::ExecuteAttack()
{
	//컴포넌트 부착한 액터 구하기
	AMonsterBase* Owner = Cast<AMonsterBase>(GetOwner());
	if (Owner && AttackMontage)
	{
		Owner->PlayAnimMontage(AttackMontage);
	}
}

void UMeleeAttackComponent::CheckHit()
{
	AMonsterBase* Owner = Cast<AMonsterBase>(GetOwner());
	if (!Owner || !CachedStatComp)
	{
		return;
	}
	Damage = CachedStatComp->GetAttackDamage();
	Range = CachedStatComp->GetAttackRange();
	AttackRadius = 25.0f;
	
	FVector Start = Owner->GetActorLocation();
	FVector Forward = Owner->GetActorForwardVector();
	FVector End = Start + ( Forward * Range);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	
	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(AttackRadius), Params);
	Owner->RotateToPlayerTarget();
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitResult.GetActor()->IsValidLowLevel())
		{
			
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
			if (PlayerPawn)
			{
				UBattleSubsystem* BattleSubsystem = GetWorld()->GetGameInstance() ? GetWorld()->GetGameInstance()->GetSubsystem<UBattleSubsystem>() : nullptr;
				if (BattleSubsystem)
				{
					if (HitActor == PlayerPawn)
					{
						BattleSubsystem->ExecuteDamageCalculation(
				GetOwner(), 
				PlayerPawn, 
						Damage, 
				false, 
				1
						);
					}
				}
			}
		}
	}
}

