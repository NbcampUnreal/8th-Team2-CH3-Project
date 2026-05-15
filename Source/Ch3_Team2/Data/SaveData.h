#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

UCLASS()
class CH3_TEAM2_API USaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(SaveGame) int32 SaveVersion = 2;
	
	UPROPERTY(SaveGame) int32 PlayerLevel = 0;
	
	UPROPERTY(SaveGame) int32 Character = 0; // Enum
	UPROPERTY(SaveGame) int32 Weapon = 0; // Enum
	
	UPROPERTY(SaveGame) float Stage1Time = 0;
	UPROPERTY(SaveGame) float Stage2Time = 0;
	UPROPERTY(SaveGame) float Stage3Time = 0;
	
	UPROPERTY(SaveGame) int32 MeleeKills = 0;
	UPROPERTY(SaveGame) int32 RangedKills = 0;
	UPROPERTY(SaveGame) int32 EliteMeleeKills = 0;
	UPROPERTY(SaveGame) int32 EliteRangedKills = 0;
	UPROPERTY(SaveGame) int32 BossKills = 0;
	
	UPROPERTY(SaveGame) int32 TotalDamage = 0;
	
    UPROPERTY(SaveGame) int32 Currency = 0;
};
