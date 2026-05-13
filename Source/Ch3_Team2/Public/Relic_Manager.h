#pragma once

#include "CoreMinimal.h"
#include "RelicData.h"
#include "GameFramework/Actor.h"
#include "Relic_Manager.generated.h"

UCLASS()
class ARelic_Manager : public AActor
{
	GENERATED_BODY()
	
public:
	ARelic_Manager();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Relic")
	TArray<FRelicData> OwnedRelics;
	//중복 일반,희귀만 가능
	
	
	void AddOwnedRelic(const FRelicData& NewRelic);
	
};
