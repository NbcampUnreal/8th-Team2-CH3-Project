
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RelicEffectBase.generated.h"

UCLASS(Blueprintable)
class CH3_TEAM2_API URelicEffectBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	virtual void ApplyEffect(UObject* WorldContextObject,const FRelicData& NewRelic);
	
};