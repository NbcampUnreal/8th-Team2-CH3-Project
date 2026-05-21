// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossSkillComponent.generated.h"

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH3_TEAM2_API UBossSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBossSkillComponent();
	UFUNCTION(BlueprintCallable, Category = "Boss Skill|Power Slam")
	void PlayPowerSlam();

	// 애니메이션 노티파이(Anim Notify)가 터졌을 때 C++에서 호출할 실제 판정 함수
	UFUNCTION(BlueprintCallable, Category = "Boss Skill|Power Slam")
	void ExecutePowerSlamHit();
	
	UFUNCTION(BlueprintCallable, Category = "Boss Skill|Plasma Orb")
	void PlayPlasmaOrb();

	UFUNCTION(BlueprintCallable, Category = "Boss Skill|Plasma Orb")
	void ExecuteSpawnPlasmaOrb();
	
	UPROPERTY(EditAnywhere, Category = "Boss Skill|Plasma Orb")
	TSubclassOf<class ABossPlasmaOrb> PlasmaOrbClass;
	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<AActor> PoolManagerClass;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category = "Boss Skill|Power Slam")
	UAnimMontage* SkillMontage;
	
	UPROPERTY(EditAnywhere, Category = "Boss Skill|Plasma Orb")
	UAnimMontage* PlasmaMontage;
	
	AActor* FoundActor;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill|Power Slam", meta = (AllowPrivateAccess = "true"))
	float SlamDamage = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill|Power Slam", meta = (AllowPrivateAccess = "true"))
	float SlamRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Skill|Power Slam", meta = (AllowPrivateAccess = "true"))
	float ForwardOffset = 200.0f;
};
