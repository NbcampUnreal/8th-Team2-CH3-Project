// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class CH3_TEAM2_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	// Gun SkeletalMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* GunMesh;
	
	// 발사 위치.... 이건  흠..
	// 수정 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> FirePoint;

	//기본 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category= "Stat")
	float BaseDamage =  25;
	
	// 성유물로 증가한 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float RelicBonus = 0;
	
	// 전체 공격력 증가 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float TotalBonus = 1.0f;

	// 최종 공격력 ( 계산 다된거 )  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float FinalDamage = RelicBonus + BaseDamage;
	
	// 크리티컬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float CritMultiplier = 2.0f;

	
	//탄약 보유량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	int32 MaxAmmo = 12;
	//남은 탄약수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category= "Stat")
	int32 CurrentAmmo = MaxAmmo;

	//연사속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float RoundsPerSecond = 1.0f;

	//유효사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float EffectiveRange = 1000.0f;
	
	// 사격 가능 여부 
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "Stat")
	bool CanFire = true;

	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	FVector2D SpreadAngle;
	
	// 장전시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float ReloadTime = 1.2f;
	
	// 집탄률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stat")
	float SpreadAngleDegrees = 12.0f;
	
	// 현제 장전 여부
	bool ReloadingCheck = false;
	// 장전 TimeHandle
	FTimerHandle TimeReloadDelay;
	
	//연사속도 제어를 위한 핸들
	FTimerHandle TimerFireDelay;
};