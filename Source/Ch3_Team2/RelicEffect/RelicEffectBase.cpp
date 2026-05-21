
#include "RelicEffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "RelicData.h"
#include "Ch3_Team2/APlayer.h"

void URelicEffectBase::ApplyRelic(UObject* WorldContextObject,const FRelicData& Relic)
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
	if (!Player) return;
	
	switch (Relic.RelicStat)
	{
	case ERelicStatType::MaxHP:
		Player->AddMaxHp(Relic.Value);
		break;
	case ERelicStatType::MoveSpeed:
		Player->AddPlayerSpeed(Relic.Value);
		break;
	case ERelicStatType::AmmoDamage:
		Player->TotalDamageUpGrade(Relic.Value,0,0);
		break;
	case ERelicStatType::SkillCooldown:
		Player->DegreaseSkillCoolTime(Relic.Value);
		break;
	case ERelicStatType::critical:
		{
			float CurrentValue = Relic.Value * 0.01;
			Player->TotalDamageUpGrade(0,0,CurrentValue);
			break;
		}
	default:
		break;
	}
}
