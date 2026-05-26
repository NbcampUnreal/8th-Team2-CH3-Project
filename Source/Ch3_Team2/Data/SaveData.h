#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

UCLASS()
class CH3_TEAM2_API USaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(SaveGame) int32 SaveVersion = 3;
	
	UPROPERTY(SaveGame) TArray<int32> RelicIDs;
	
	UPROPERTY(SaveGame) int32 PlayerLevel = 0; 
	UPROPERTY(SaveGame) int32 PlayerSkill = 0;
	UPROPERTY(SaveGame) int32 PlayerWeapon = 0;
	
	UPROPERTY(SaveGame) int32 GripLevel = 0;
	UPROPERTY(SaveGame) int32 ScopeLevel = 0;
	UPROPERTY(SaveGame) int32 MagazineLevel = 0;
	UPROPERTY(SaveGame) int32 BulletLevel = 0;
	
	UPROPERTY(SaveGame) TArray<float> StageClearTime = {0.f, 0.f, 0.f};
	
	UPROPERTY(SaveGame) int32 MeleeKills = 0;
	UPROPERTY(SaveGame) int32 RangedKills = 0;
	UPROPERTY(SaveGame) int32 EliteMeleeKills = 0;
	UPROPERTY(SaveGame) int32 EliteRangedKills = 0;
	UPROPERTY(SaveGame) int32 BossKills = 0;
	
	UPROPERTY(SaveGame) int32 TotalDamage = 0;
};
