#include "SkillBaseComp.h"

USkillBaseComp::USkillBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillBaseComp::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	CurrentSkillCoolTime = 0.0f;
}
void USkillBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CoolDownSkill(DeltaTime);
}
void USkillBaseComp::ActiveSkill()
{
	if (CurrentSkillCoolTime <= 0.0f )
	{
		// 스킬 Component tick 함수 활성화 
		SetComponentTickEnabled(true);
		// 비 활성화 하는 TimeHandle 
		CurrentSkillCoolTime = SkillCoolTime;
	}
}
void USkillBaseComp::CoolDownSkill(float DeltaTime)
{
	// 쿨타임이 0초 아래로 떨어지지 않았으면 쿨타임 계속해서 감소 
	if (CurrentSkillCoolTime > 0.0f)
		CurrentSkillCoolTime -= DeltaTime;
	else
		// 만약에 스킬  쿨 타임이 다 찼다면 Tick 함수 비활성화
			SetComponentTickEnabled(false);
}

void USkillBaseComp::DecreaseTimeSkill(float Decrease)
{
	SkillCoolTime = FMath::Max(SkillCoolTime - Decrease, MinimumSkillTime);
}

