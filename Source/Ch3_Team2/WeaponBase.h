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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage =  25;
	
	// 성유물로 증가한 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RelicBonus = 0;
	
	// 전체 공격력 증가 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalBonus = 1.0f;

	// 최종 공격력 ( 계산 다된거 )  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FinalDamage = RelicBonus + BaseDamage;
	
	// 크리티컬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CritMultiplier = 2.0f;

	
	//탄약 보유량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo = 12;
	//남은 탄약수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 CurrentAmmo = MaxAmmo;


	//연사속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoundsPerSecond = 1.0f;

	//유효사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectiveRange = 1000.0f;
	
	// 사격 가능 여부 
	UPROPERTY(BlueprintReadWrite)
	bool CanFire = true;

	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SpreadAngle;
	
	// 장전시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime = 1.2f;
	
	// 현제 장전 여부
	bool ReloadingCheck = false;
	// 장전 TimeHandle
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimeReloadDelay;
	
	//연사속도 제어를 위한 핸들
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerFireDelay;
};