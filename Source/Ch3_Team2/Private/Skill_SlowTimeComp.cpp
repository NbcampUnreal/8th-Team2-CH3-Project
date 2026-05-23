#include "Skill_SlowTimeComp.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

void USkill_SlowTimeComp::ActiveSkill()
{
	Super::ActiveSkill();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), WorldTime);

	// 정상속도로 플레이 되어야 하는 객체들
	AActor* Owner = GetOwner();
	Owner->CustomTimeDilation = 1.0f / WorldTime;
	AGameState * State = GetWorld()->GetGameState<AGameState>();
	State->CustomTimeDilation = 1.0f / WorldTime;
	
	// 종료 함수 호출
	GetWorld()->GetTimerManager().SetTimer(	SkillTimerHandle
			,this
			,&USkill_SlowTimeComp::EndSkill
			,ActiveSkillTime *WorldTime
			,false
			);
}

void USkill_SlowTimeComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void USkill_SlowTimeComp::EndSkill()
{
	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	AActor* Owner = GetOwner();
	Owner->CustomTimeDilation = 1.0f;
	AGameState * State = GetWorld()->GetGameState<AGameState>();
	State->CustomTimeDilation = 1.0f;
	
}
