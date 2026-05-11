// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterTypes.h"
#include "Poolable.h"
#include "MonsterBase.generated.h"

UCLASS()
class CH3_TEAM2_API AMonsterBase : public ACharacter,public IPoolable
{
	GENERATED_BODY()

public:
	AMonsterBase();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawn")
	class AMonsterSpawner* MonsterSpawner;
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnReadyToReturn OnMonsterDeath;
	virtual FOnReadyToReturn& GetOnReadyToReturn() override;
	
	//스텟 초기화
    	void SetMonsterStats(const FMonsterStats& InStats);
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	FMonsterStats BaseStats;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Stats")
	FMonsterStats CurrentStats;


	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	FTimerHandle DeathTimerHandle;
	void OnDeath();
	void AfterDeath();
	
	
	virtual void OnSpawnFromPool(const FTransform& Transform) override;
	virtual void OnReturnToPool() override;
};
