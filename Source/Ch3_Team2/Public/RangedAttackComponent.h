// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAttackComponent.h"
#include "RangedAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API URangedAttackComponent : public UMonsterAttackComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<class AMonsterProjectile> ProjectileClass;

	virtual void ExecuteAttack() override;
	
	virtual void CheckHit() override; 
	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<AActor> PoolManagerClass;
protected:
	virtual void BeginPlay() override;
	AActor* FoundActor;
};
