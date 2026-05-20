#pragma once
#include "CoreMinimal.h"
#include "SkillBaseComp.h"
#include "SkillOverDrive.generated.h"
class AGunBase;
UCLASS()
class CH3_TEAM2_API USkillOverDrive : public USkillBaseComp
{
	GENERATED_BODY()
public:
	UPROPERTY()
	
	float Percent = 0.5f;
	float SaveSpeed = 0;
	float SaveReload = 0;
	
	
	virtual void ActiveSkill()override;
	void EndSkill();
};