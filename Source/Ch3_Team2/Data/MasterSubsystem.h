#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MasterSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnBattleResult, 
	int32, MeleeKills,		int32, RangedKills,
	int32, EliteMeleeKills,	int32, EliteRangedKills,
	int32, BossKills,		int32, GlobalTotalDamage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyChanged, int32, NewAmount, int32, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveTime, int32, StageIndex, float, ClearTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);

UCLASS()
class CH3_TEAM2_API UMasterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FOnBattleResult OnBattleResult;
	
	UPROPERTY()
	FOnCurrencyChanged OnCurrencyChanged;
	
	UPROPERTY()
	FOnSaveTime OnSaveTime;
	
	UPROPERTY()
	FOnGameEnd OnGameEnd;
};
