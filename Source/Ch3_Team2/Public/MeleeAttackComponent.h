// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAttackComponent.h"
#include "MeleeAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API UMeleeAttackComponent : public UMonsterAttackComponent
{
	GENERATED_BODY()
public:
	virtual void ExecuteAttack() override;
	virtual void CheckHit() override;
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	class UMonsterStatComponent* CachedStatComp;
};
