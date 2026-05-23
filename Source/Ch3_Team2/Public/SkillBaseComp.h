#pragma once

#include "CoreMinimal.h"
#include "SkillInterface.h"
#include "Components/ActorComponent.h"
#include "SkillBaseComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API USkillBaseComp : public UActorComponent ,public ISkillInterface
{
	GENERATED_BODY()

public:	
	USkillBaseComp();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltDaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 액티브 스킬 사용후 재사용 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCoolTime = 20.0f;

	// 현제  스킬 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CurrentSkillCoolTime = 0.0f;
	
	//스킬 사용여부 SkillInputKey 에 추가해서 중복 스킬 잠가버리기
	bool bSkillActiveCheck =true; 
	
	//스킬 효과가 유지되는 지속 시간
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Skill")
	float ActiveSkillTime =5.0f;
	
	float MinimumSkillTime = 0.5f;
	
	// 스킬 시전 지속시간 스킬handle
	FTimerHandle SkillTimerHandle;
	
	virtual void ActiveSkill();
	virtual void CoolDownSkill(float DeltaTime);
	
	void DecreaseTimeSkill(float Decrease);
};
