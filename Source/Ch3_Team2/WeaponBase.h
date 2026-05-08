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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> FirePoint;

	UFUNCTION(BlueprintCallable) //BlueprintImplementableEvent
	virtual void Fire();
	
public:

	//소모되는 탄약수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoPerFire;

	//남은 탄약수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 CurrentAmmo;

	//탄약 보유량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;

	//연사속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoundsPerSecond;

	//유효사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectiveRange;

	//데미지양
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerHit;

	//쏠수있나
	UPROPERTY(BlueprintReadWrite)
	bool CanFire;

	//연사속도 제어를 위한 핸들
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerFireDelay;
	
	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SpreadAngle;
	
	UFUNCTION()
	void HandleFireDelay();
	
	//PartsLevel 부품은 일단 Keep

};
