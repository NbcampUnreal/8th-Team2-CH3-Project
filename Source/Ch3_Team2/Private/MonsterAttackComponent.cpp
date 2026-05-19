// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackComponent.h"
#include "MonsterStatComponent.h"
UMonsterAttackComponent::UMonsterAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}
void UMonsterAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* Owner = GetOwner())
	{
		//몬스터가 가진 스텟컴포넌트 주소 시작할때 한번 저장해서 사용
		CachedStatComp = Owner->FindComponentByClass<UMonsterStatComponent>();
	}
}
void UMonsterAttackComponent::ExecuteAttack()
{
}

void UMonsterAttackComponent::CheckHit()
{
}

