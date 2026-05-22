#include "Skill_SlowTimeComp.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

void USkill_SlowTimeComp::ActiveSkill()
{
	Super::ActiveSkill();
	// 1. 전역 시간을 0.2배속으로 (세상 모든 것이 느려짐)
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), WorldTime);

	// 정상속도로 플레이 되어야 하는 객체들
	AActor* Owner = GetOwner();
	Owner->CustomTimeDilation = 1 / WorldTime;
	AGameState * State = GetWorld()->GetGameState<AGameState>();
	State->CustomTimeDilation = 1 / WorldTime;
	
	// 종료 함수 호출
	GetWorld()->GetTimerManager().SetTimer(	SkillTimerHandle
			,this
			,&USkill_SlowTimeComp::EndSkill
			,ActiveSkillTime *WorldTime
			,false
			);
	
}
	

void USkill_SlowTimeComp::EndSkill()
{
	// 스킬 Time 헨들러 초기화
	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
	// 스킬 종료 함수 호출
	// 시간 정상화
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	AActor* Owner = GetOwner();
	Owner->CustomTimeDilation = 1.0f;
	AGameState * State = GetWorld()->GetGameState<AGameState>();
	State->CustomTimeDilation = 1.0f;
	
}
