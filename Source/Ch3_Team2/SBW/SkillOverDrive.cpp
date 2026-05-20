#include "SBW/SkillOverDrive.h"
#include "APlayer.h"
#include "GunBase.h"
#include "APlayer.h"

void USkillOverDrive::ActiveSkill()
{
	if (SkillActiveCheck == false) return;
	
	AAPlayer* PlayerOwner = Cast<AAPlayer>(GetOwner());
	if (!PlayerOwner) return;

	if (PlayerOwner->ChildActor)
	{
		AGunBase* CachedGun = Cast<AGunBase>(PlayerOwner->ChildActor->GetChildActor());
		// 무기가 성공적으로 캐싱되었다면 스킬 로직을 실행합니다.
		if (CachedGun)
		{
			//float CurrentSpeed = GetOwner()->GetActorSpeed();
			SaveReload = CachedGun->GetReloadSpeed();
			CachedGun->AddReloadStat(SaveReload * Percent);
			
			SaveSpeed = PlayerOwner->GetSpeed();
			float Test = SaveSpeed * Percent;
 			PlayerOwner->AddPlayerSpeed(Test);
			SkillActiveCheck = false;
			

			// 타이머 설정
			GetWorld()->GetTimerManager().SetTimer(
				SkillTimerHandle,
				this,
				&USkillOverDrive::EndSkill,
				ActiveSkillTime,
				false
			);
		}
	}
	Super::ActiveSkill();
}

void USkillOverDrive::EndSkill()
{
	AAPlayer* PlayerOwner = Cast<AAPlayer>(GetOwner());
	if (!PlayerOwner) return;

	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
	AGunBase* CachedGun = Cast<AGunBase>(PlayerOwner->ChildActor->GetChildActor());
	if (CachedGun)
	{
		// [스탯 원상 복구]
		PlayerOwner->AddPlayerSpeed(SaveSpeed - PlayerOwner->GetSpeed());
		CachedGun->AddReloadStat(SaveReload - CachedGun->GetReloadSpeed()); 
	}
	CachedGun = nullptr;
	SkillActiveCheck= true;
}
