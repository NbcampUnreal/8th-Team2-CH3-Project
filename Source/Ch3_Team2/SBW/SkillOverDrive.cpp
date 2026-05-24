#include "SBW/SkillOverDrive.h"
#include "APlayer.h"
#include "GunBase.h"
#include "APlayer.h"

void USkillOverDrive::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
	
	Super::EndPlay(EndPlayReason);
}

void USkillOverDrive::ActiveSkill()
{
	Super::ActiveSkill();
	if (bSkillActiveCheck == false)
	{
		return;
	}
	AAPlayer* PlayerOwner = Cast<AAPlayer>(GetOwner());
	if (!PlayerOwner)
	{
		return;
	}

	if (PlayerOwner->ChildActor)
	{
		AGunBase* CachedGun = Cast<AGunBase>(PlayerOwner->ChildActor->GetChildActor());
		// 무기가 성공적으로 캐싱되었다면 스킬 로직을 실행합니다.
		if (CachedGun)
		{
			//float CurrentSpeed = GetOwner()->GetActorSpeed();
			SaveReload = CachedGun->GetReloadSpeed();
			SaveReload *= Percent;
			CachedGun->DecreaseReloadTimeStat(SaveReload);
			
			SaveSpeed = PlayerOwner->GetSpeed();
			SaveSpeed *= Percent;
			
 			PlayerOwner->AddPlayerSpeed(SaveSpeed);
			bSkillActiveCheck = false;

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
}

void USkillOverDrive::EndSkill()
{
	GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
	AAPlayer* PlayerOwner = Cast<AAPlayer>(GetOwner());
	if (!PlayerOwner)
	{
		return;
	}

	AGunBase* CachedGun = Cast<AGunBase>(PlayerOwner->ChildActor->GetChildActor());
	if (CachedGun)
	{
		// [스탯 원상 복구]
		PlayerOwner->AddPlayerSpeed(-SaveSpeed);
		CachedGun->DecreaseReloadTimeStat(-SaveReload); 
		SaveSpeed = 0.0f;
		SaveReload = 0.0f;
	}
	bSkillActiveCheck= true;
}
