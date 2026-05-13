#include "Relic_Manager.h"


ARelic_Manager::ARelic_Manager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARelic_Manager::AddOwnedRelic(const FRelicData& NewRelic)
{
	OwnedRelics.Add(NewRelic);
}
