
#pragma once

#include "CoreMinimal.h"
#include "SkillBaseComp.h"
#include "Skill_SlowTimeComp.generated.h"

UCLASS()
class CH3_TEAM2_API USkill_SlowTimeComp : public USkillBaseComp
{
	GENERATED_BODY()
public:
	virtual void ActiveSkill()override;
	void EndSkill();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= "Time")
	float WorldTime = 0.2f;
	
};