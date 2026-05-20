// Fill out your copyright notice in the Description page of Project Settings.


#include "SBW/SkillOverDrive.h"
#include "APlayer.h"
#include "GunBase.h"

void USkillOverDrive::ActiveSkill()
{
	/*
	// 1. 이 컴포넌트를 소유한 액터(플레이어)를 가져와 캐스팅합니다.
	AAPlayer* PlayerOwner = Cast<AAPlayer>(GetOwner());
	if (!PlayerOwner) return;

	// 2. 플레이어가 가진 ChildActorComponent가 유효한지 확인합니다.
	if (PlayerOwner->ChildActor)
	{
		// 3. ChildActorComponent가 스폰한 실제 액터를 AGunBase로 캐스팅합니다.
		AGunBase* MyGunBase = Cast<AGunBase>(PlayerOwner->ChildActor->GetChildActor());
		if (MyGunBase)
		{
			// 이제 무기의 데이터나 함수를 마음껏 호출할 수 있습니다!
			// 예시: 탄창 수 조절 및 발사 함수 호출 등
			int32 MaxAmmo = MyGunBase->GetMaxAmmoStat();
			UE_LOG(LogTemp, Warning, TEXT("현재 무기의 최대 탄 수: %d"), MaxAmmo);
            
			// 필요하다면 전역 변수로 저장해두고 쓸 수도 있습니다.
			MyGun = MyGunBase; 
		}
	}
	 */
	
	Super::ActiveSkill();
}
