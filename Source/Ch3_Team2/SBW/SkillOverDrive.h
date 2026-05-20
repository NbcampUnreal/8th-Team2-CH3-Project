// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBaseComp.h"
#include "SkillOverDrive.generated.h"

/**
 * 
 */
UCLASS()
class CH3_TEAM2_API USkillOverDrive : public USkillBaseComp
{
	GENERATED_BODY()
public:
	//AActor* MyGun;
	virtual void ActiveSkill();
};
