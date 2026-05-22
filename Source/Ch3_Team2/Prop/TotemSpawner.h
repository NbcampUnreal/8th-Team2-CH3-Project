#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TotemSpawner.generated.h"

class AHealTotem;

UCLASS()
class CH3_TEAM2_API ATotemSpawner : public AActor
{
	GENERATED_BODY()
	
public: 
	ATotemSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPoolComponent* PoolComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnerSettings")
	TSubclassOf<AHealTotem> TotemClass;

	// 게임 시작할 때 풀에 미리 만들어둘 토템 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnerSettings")
	int32 PreAllocateCount = 11;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnerSettings")
	int32 TargetSpawnCount = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnerSettings")
	float MinDistanceBetweenTotems = 1000.0f;
	
	int32 CurrentActiveTotems = 0;

	UPROPERTY(EditAnywhere, Category = "SpawnerSettings")
	float SpawnBound = 25000.0f;
    
	UFUNCTION()
	void OnTotemReturned(AActor* ReturnedActor);
	
	void PreAllocateTotems();
	void RandomSpawnTotem();
	
	AHealTotem* SpawnHealTotem(const FTransform& Transform);
	
private:
	// 현재 필드에 살아있는(활성화된) 토템들만 담아둘 배열
	UPROPERTY()
	TArray<AHealTotem*> ActiveTotemList;
};
