
#include "RelicEffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "RelicData.h"
#include "Ch3_Team2/APlayer.h"

void URelicEffectBase::ApplyEffect(UObject* WorldContextObject,const FRelicData& Relic)
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
	
	if (!Player)
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
	}
}
