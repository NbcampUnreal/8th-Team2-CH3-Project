// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this component's properties
	USkillBaseComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ----- base 데이터만 설정해놓기
	
	// 플레이어 데이터 입력시 같이 넣을 예정
	void SkillDataReset();
	
	// 액티브 스킬 사용후 재사용 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCoolTime;

	// 현제  스킬 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CurrentSkillCoolTime;
	
	//스킬 사용여부 SkillInputKey 에 추가해서 중복 스킬 잠가버리기
	bool SkillActiveCheck; 
	
	//스킬 효과가 유지되는 지속 시간
	const float ActiveSkilltime =5.0f;
	
	// 스킬 시전 지속시간 스킬handle
	FTimerHandle SkillTimerHandle;
	
	virtual void ActiveCheck();
		
	// 스킬 활성화 
	// 에니메이션 노티파이.
	virtual void SkillActive();
	
	// 스킬 시간종료 
	virtual void SkillEnd();
	
	// 스킬 쿨타임 시간 감소 
	virtual void SkillCoolDown(float DeltaTime);
	
	void SkiilDegreaseTime(float Degrease);
	
};
