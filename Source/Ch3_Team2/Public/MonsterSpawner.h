// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterBase.h"
#include "Engine/DataTable.h"
#include "MonsterSpawner.generated.h"

USTRUCT()
struct FMonsterPool
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<class AMonsterBase*> InactiveMonsters;
};

USTRUCT(BlueprintType)
struct FMonsterSpawnConfig : public FTableRowBase
{
	GENERATED_BODY()

	// 소환할 몬스터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AMonsterBase> MonsterClass;

	// 미리 생성해둘 개수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PreAllocateCount = 10;
	
	//몬스터 스텟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMonsterStats MonsterStats;
};

UCLASS()
class CH3_TEAM2_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpawner();
	
	//미리 몬스터 만들어놓음
	void PreAllocateMonsters();
	//몬스터 소환
	AMonsterBase* SpawnMonster(TSubclassOf<AMonsterBase> MonsterClass,const FTransform& Transform,const FMonsterStats& InStats);
	void RandomSpawnMonster();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	class UPoolComponent* PoolComp;
	
	//소환할 몬스터 종류
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Setting")
	UDataTable* MonsterTable;
	
	//미리 Table에서 정보를 읽어옴
	TArray<FMonsterSpawnConfig*> MonsterConfigs;
	
	FTimerHandle SpawnTimerHandle;
private:
	float SpawnBound;
};
