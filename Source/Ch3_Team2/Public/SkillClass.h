// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class SkillType
{
	SlowSkill
	// 차차 추가 하기로.
};
class CH3_TEAM2_API SkillClass
{
public:
	SkillClass();
	~SkillClass();
	
	virtual void Tick(float DeltaTime);
	// 스킬 활성화 
	void ActiveSkill();
	
	// 스킬 초기화
	void InitSkill();
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCoolTime;

	// 현제  스킬 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CurrentSkillCoolTime;
	
	bool SkillActiveCheck; //스킬 사용여부 SkillInputKey 에 추가해서 중복 스킬 잠가버리기
	//스킬 효과가 유지되는 지속 시간
	const float ActiveSkilltime =5.0f;
	
	// 스킬 시전 지속시간 스킬handle
	FTimerHandle SkillTimerHandle;
	// 현제 적용중인 스킬 타입 확인
	SkillType SkillTypeCheck;
	// 속도 감소 스킬 활성화
	void SkillTimeSlow();
	
	//  감소된 속도 정상화 
	void SkillTimeNormal();
	
	void ActivateSkillCooldown(float DeltaTime);
};
