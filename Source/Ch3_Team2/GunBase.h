// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GunParts.h"
#include "GunBase.generated.h"

UCLASS()
class CH3_TEAM2_API AGunBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	AGunBase();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category= "Weapon")
	void Reload();
	void Reload_End();

	// 매개 변수 설정
	virtual void Stats_Initialize();
	
	void InitializeParts();
	
	// 블루프린트 위임
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckAmmo();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire_Gun(FVector Location, FVector Direction);
	
	// RPM ( 연사 속도 )시간 끝내는함수 
	void HandleFireDelay();
	void AddDamage(float Add_RelicDamage,float Add_TotalDamage,float Critical);
	
	void BattleIn(const FHitResult& HitResult);
	void SelectParts(EPartsName parts);
	
	// Weapon Parts
	FGunParts Bullet;
	FGunParts Magazine;
	FGunParts Scope;
	FGunParts Handle;
	
protected:
	virtual void BeginPlay() override;
};
