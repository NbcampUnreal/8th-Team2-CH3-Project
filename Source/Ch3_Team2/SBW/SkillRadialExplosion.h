// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBaseComp.h"
#include "SkillRadialExplosion.generated.h"

/**
 * 
 */
class UCollisionCompoent;

UCLASS()
class CH3_TEAM2_API USkillRadialExplosion : public USkillBaseComp
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= "SkillStat")
	float DetectionRadius = 1000.0f; // 인식 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= "SkillStat")
	float LaunchZDirection = 1.2f; // 넉백시 살짝 밀려나는 z값
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= "SkillStat")
	float KnockbackStrength = 1500.0f;// 밀려나는 힘 (수치 조절 필요)
	
	virtual void ActiveSkill()override;
};
