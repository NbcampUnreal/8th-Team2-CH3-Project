// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBaseComp.h"

// Sets default values for this component's properties
USkillBaseComp::USkillBaseComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Called when the game starts
void USkillBaseComp::BeginPlay()
{
	Super::BeginPlay();
	SkillDataReset();
}


// Called every frame
void USkillBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Tick함수 정상적으로 호출 또는 비활성화 되는지 확인용
	GEngine->AddOnScreenDebugMessage(
	-1,	3.0f,FColor::Yellow,
	FString::Printf(TEXT("Skill Active: Cool %f"), CurrentSkillCoolTime));
	
	// 스킬 쿨타임 확인용
	SkillCoolDown(DeltaTime);
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillBaseComp::SkillDataReset()
{
	// 스킬 관련
	SkillCoolTime = 20.0f;
	//
	//ActiveSkilltime = 5.0f; - const화 해서 나중에 수정가능하게 할려면 const 제거 하고 할것
	CurrentSkillCoolTime= 0;
	SkillActiveCheck =true;
}

void USkillBaseComp::ActiveCheck()
{
	// 스킬 쿨타임이 0 초 이고 SkillActiveCheck 유무가 true 일때 
	// 스킬 사용가능하도록 설정
	// 시간 연장 개념이면 다르게 설정해야할지도.
	
	// 변수를 포함해서 출력하고 싶을 때
	FString SkillName = TEXT("시간 감소!");
	float Cooldown = 5.5f;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
		FString::Printf(TEXT("스킬: %s, 남은 시간: %.1f"), *SkillName, Cooldown));
	
	if (CurrentSkillCoolTime <= 0.0f )
	{
		// 스킬 시전
		
		// 스킬 시전
		SkillActive();		
	}
}

void USkillBaseComp::SkillActive()
{
	SetComponentTickEnabled(true);
	
	// 비 활성화 하는 TimeHandle 
	CurrentSkillCoolTime = SkillCoolTime;
}

void USkillBaseComp::SkillEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
}

void USkillBaseComp::SkillCoolDown(float DeltaTime)
{
	// 쿨타임이 0초 아래로 떨어지지 않았으면 쿨타임 계속해서 감소 
	if (CurrentSkillCoolTime > 0.0f)
		CurrentSkillCoolTime -= DeltaTime;
	else
		// 만약에 스킬  쿨 타임이 다 찼다면 Tick 함수 비활성화
			SetComponentTickEnabled(false);
}

