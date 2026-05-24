#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MasterSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnBattleResult, 
	int32, MeleeKills,		int32, RangedKills,
	int32, EliteMeleeKills,	int32, EliteRangedKills,
	int32, BossKills,		int32, GlobalTotalDamage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveTime, int32, StageIndex, float, ClearTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveRelic, TArray<int32>, RelicIDs);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSavePlayer, int32, PlayerLevel, int32, PlayerSkill, int32, PlayerWeapon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSaveGun, int32, GripLevel, int32, ScopeLevel, int32, MagazineLevel, int32, BulletLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEliteMonsterKills);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossMonsterKills);

UCLASS()
class CH3_TEAM2_API UMasterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FOnBattleResult OnBattleResult;
	
	UPROPERTY()
	FOnSaveTime OnSaveTime;
	
	UPROPERTY()
	FOnSaveRelic OnSaveRelic;
	
	UPROPERTY()
	FOnSavePlayer OnSavePlayer;
	
	UPROPERTY()
	FOnSaveGun OnSaveGun;
	
	UPROPERTY()
	FOnGameEnd OnGameEnd;
	
	UPROPERTY()
	FOnEliteMonsterKills OnEliteMonsterKills;
	
	UPROPERTY()
	FOnBossMonsterKills OnBossMonsterKills;
};
