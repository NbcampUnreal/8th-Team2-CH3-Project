#pragma once

#include "CoreMinimal.h"
#include "RelicData.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "RelicApplyManager.h"
#include "RelicManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelicRewardGeneratedSignature, const TArray<FRelicData>&, RewardChoices);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEliteMonsterDeathSignature, const AActor*, DeathActor);

UCLASS()
class ARelicManager : public AActor
{
	GENERATED_BODY()
	
public:
	ARelicManager();
	
	//플레이어가 현재가지고 있는 유물 배열
	TArray<TPair<int32,bool>> OwnedRelicIDs;
	//에픽 레전드는 여기에 아이디 저장으로 중복 방지
	UPROPERTY()
	TSet<int32> BlockedRelicIDs;
	//UI에 넘겨줄 랜덤 3개 성유물 배열
	UPROPERTY()
	TArray<FRelicData> RandomRelicOption;
	// 데이터 테이블에서 읽어온 전체 유물 목록
	UPROPERTY()
	TArray<FRelicData> AllRelics;
	//중복 방지
	UFUNCTION()
	bool GetRandomRelicByGrade(
		ERelicGrade Grade,
		FRelicData& OutRelic);
	
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void AddOwnedRelic(const FRelicData& NewRelic);
	
	//데이터 테이블 받기
	UPROPERTY(EditAnywhere,Category = "Relic")
	UDataTable* RelicDataTable;
	
	UPROPERTY(EditAnywhere, Category = "Relic")
	TSubclassOf<ARelicApplyManager> ApplyManagerClass;
	
	UPROPERTY()
	ARelicApplyManager* ApplyManager;
	
	void RandomRelic();
	
	UPROPERTY(BlueprintAssignable, Category = "Relic")
	FOnRelicRewardGeneratedSignature OnRelicRewardGenerated;
	
	ERelicGrade NormalRollGrade();
	
	void LodeData(TArray<int32> RelicIDs);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY()
	class UMasterSubsystem* MasterSubsystem;;
	
protected:
	
	virtual void BeginPlay() override;
};


