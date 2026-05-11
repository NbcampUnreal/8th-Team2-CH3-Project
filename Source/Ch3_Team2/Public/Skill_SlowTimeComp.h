// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBaseComp.h"
#include "Skill_SlowTimeComp.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API USkill_SlowTimeComp : public USkillBaseComp
{
	GENERATED_BODY()
	
	virtual void ActiveCheck()override;
	virtual void SkillActive()override;
	virtual void SkillEnd();
	
};