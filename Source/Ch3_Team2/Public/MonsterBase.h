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
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawn")
	class AMonsterSpawner* MonsterSpawner;
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnReadyToReturn OnMonsterDeath;
	virtual FOnReadyToReturn& GetOnReadyToReturn() override;
	
	//스텟 초기화
	void SetMonsterStats(const FMonsterStats& InStats);
	
	//스텟 컴포넌트 읽기
	FORCEINLINE class UMonsterStatComponent* GetStatComponent() const { return StatComp; }
	
	UFUNCTION(BlueprintCallable)
	void PlayDeathAnim();
	
	UFUNCTION(BlueprintCallable)
	void BossAfterDeath();
	
	void RotateToPlayerTarget();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMonsterStatComponent* StatComp;
	//블루프린트에서 추가해줌
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMonsterAttackComponent* AttackComp;
	
	FTimerHandle DeathTimerHandle;
	UFUNCTION()
	void HandleDeath(AController* InInstigator,AActor* DeathActor);
	void AfterDeath();
	
	//exp아이템
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Drop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ExpItemClass;
	UFUNCTION()
	void DropExpItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Drop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PortalClass;
	void SpawnBossPortal();
	
	UPROPERTY(EditAnywhere,Category = "Animation")
	UAnimMontage* DeathMontage;

	virtual void OnSpawnFromPool(const FTransform& Transform) override;
	virtual void OnReturnToPool() override;
};
