#pragma once

#include "CoreMinimal.h"
#include "RelicData.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RelicManager.generated.h"

UCLASS()
class ARelicManager : public AActor
{
	GENERATED_BODY()
	
public:
	ARelicManager();
	
	//중복 일반,희귀만 가능 + 플레이어가 현재가지고 있는 유물 배열
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Relic")
	TArray<FRelicData> OwnedRelics;
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
	
	void AddOwnedRelic(const FRelicData& NewRelic);
	
	//데이터 테이블 받기
	UPROPERTY(EditAnywhere,Category = "Relic")
	UDataTable* RelicDataTable;
	
	void BeginPlay() override;
	void RandomRelic();
	//인풋 테스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* IMC_Relic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Relic1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Relic2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Relic3;
	
	void SelectRelic1();
	
	void SelectRelic2();
	
	void SelectRelic3();
	
	ERelicGrade NormalRollGrade();
	
};


