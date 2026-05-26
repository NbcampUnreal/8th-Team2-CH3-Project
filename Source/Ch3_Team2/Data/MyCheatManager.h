#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MyCheatManager.generated.h"

UCLASS()
class CH3_TEAM2_API UMyCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(exec)
	void Next();
	
	UFUNCTION(exec)
	void Relic();
	
	UFUNCTION(exec)
	void LevelUp();
};
