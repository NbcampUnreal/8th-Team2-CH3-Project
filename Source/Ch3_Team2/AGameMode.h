#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AGameMode.generated.h"

UCLASS()
class CH3_TEAM2_API AAGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AAGameMode();
	
protected:
	virtual void BeginPlay() override;
};
