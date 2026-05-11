// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillInterface : public UInterface
{
	GENERATED_BODY()
};

class CH3_TEAM2_API ISkillInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 스킬 사용 활성화용
	
	// virtual 을 뺴야지 사용 가능함.
	// 스킬 활성화 
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent , Category="Interface")
	void ActiveSkill(AActor *Player);
};
