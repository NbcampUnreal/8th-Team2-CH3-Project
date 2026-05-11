// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_SlowTimeComp.h"

#include "Kismet/GameplayStatics.h"

void USkill_SlowTimeComp::ActiveCheck()
{
	
	Super::ActiveCheck();
}

void USkill_SlowTimeComp::SkillActive()
{
	AActor* Owner = GetOwner();
	// 1. 전역 시간을 0.1배속으로 (세상 모든 것이 느려짐)
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
	Owner->CustomTimeDilation = 5.0f;
	
	// 종료 함수 호출
	GetWorld()->GetTimerManager().SetTimer(	SkillTimerHandle
			,this
			,&USkill_SlowTimeComp::SkillEnd
			,ActiveSkilltime *0.2f
			,false
			);
	Super::SkillActive();
}

void USkill_SlowTimeComp::SkillEnd()
{
	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
		FString::Printf(TEXT("스킬 종료 ")));
	UE_LOG(LogTemp,Warning,TEXT("스킬종료"));
	 */
	// 스킬 종료 함수 호출
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	AActor* Owner = GetOwner();
	Owner->CustomTimeDilation = 1.0f;

	Super::SkillEnd();
}
