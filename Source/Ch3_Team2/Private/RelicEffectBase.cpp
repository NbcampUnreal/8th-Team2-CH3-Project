
#include "RelicEffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "RelicData.h"
#include "Ch3_Team2/APlayer.h"
#include "Ch3_Team2/WeaponBase.h"

void URelicEffectBase::ApplyEffect(UObject* WorldContextObject,const FRelicData& Relic)
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
	AWeaponBase* Weapon = Cast<AWeaponBase>(UGameplayStatics::GetActorOfClass(GetWorld(),AWeaponBase::StaticClass()));
	if (!Player || !Weapon)
	{
		return;
	}
	
	switch (Relic.RelicStat)
	{
	case ERelicStatType::MaxHP:
		Player->AddMaxHp(Relic.Value);
		break;
	case ERelicStatType::MoveSpeed:
		Player->MoveSpeed += Relic.Value;
		break;
	case ERelicStatType::AmmoDamage:
		Weapon->AmmoDamage += Relic.Value;
		break;
	case ERelicStatType::critical:
		Weapon->CritMultiplier += Relic.Value;
		break;
	case ERelicStatType::SkillColldown:
		Player->SkillCoolTime -= Relic.Value;
		
	}
}
