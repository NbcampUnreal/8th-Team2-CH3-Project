#include "SkillBaseComp.h"

USkillBaseComp::USkillBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Called when the game starts
void USkillBaseComp::BeginPlay()
{
	Super::BeginPlay();
	ResetDataSkill();
}


// Called every frame
void USkillBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 스킬 쿨타임 확인용
	CoolDownSkill(DeltaTime);
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillBaseComp::ResetDataSkill()
{
	// 스킬 관련
	SkillCoolTime = 20.0f;
	//ActiveSkilltime = 5.0f; - const화 해서 나중에 수정가능하게 할려면 const 제거 하고 할것
	CurrentSkillCoolTime= 0;
	// 스킬 활성화 bool 값 true
	SkillActiveCheck =true;
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
	if (SkillCoolTime - Decrease > 0.5f)
	{
		SkillCoolTime-=Decrease;
		if (SkillCoolTime <= 0.5f)
			SkillCoolTime = 0.5f;
	}
}

