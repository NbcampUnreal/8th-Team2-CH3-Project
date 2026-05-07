#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelFlowDataAsset.generated.h"

UCLASS()
class CH3_TEAM2_API ULevelFlowDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// [0]은 메인 메뉴, [1]부터 게임 스테이지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Flow", meta = (AllowedClasses = "/Script/Engine.World"))
	TArray<TSoftObjectPtr<UWorld>> LevelSequence;
};