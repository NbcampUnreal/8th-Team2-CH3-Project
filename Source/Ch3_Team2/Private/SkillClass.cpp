// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillClass.h"

SkillClass::SkillClass()
{
}

SkillClass::~SkillClass()
{
}


void SkillClass::Tick(float DeltaTime)
{
}

void SkillClass::ActiveSkill()
{
}

void SkillClass::InitSkill()
{
	SkillCoolTime = 20.0f;
	//ActiveSkilltime = 5.0f; - const화 해서 나중에 수정가능하게 할려면 const 제거 하고 할것
	CurrentSkillCoolTime= 0;
}

void SkillClass::SkillTimeSlow()
{
}

void SkillClass::SkillTimeNormal()
{
}

void SkillClass::ActivateSkillCooldown(float DeltaTime)
{
}
