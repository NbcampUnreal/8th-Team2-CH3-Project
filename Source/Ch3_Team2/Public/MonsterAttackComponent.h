// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API UMonsterAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterAttackComponent();
	
	// State Tree에서 호출할 공격 명령
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAttack();

	//애니메이션 노티파이에서 호출
	UFUNCTION(BlueprintCallable)
	virtual void CheckHit();
protected:
	
	UPROPERTY(EditAnywhere,Category = "Animation")
	class UAnimMontage* AttackMontage;
};
