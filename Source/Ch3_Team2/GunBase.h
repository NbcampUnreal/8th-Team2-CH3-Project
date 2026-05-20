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
	void InitializeParts();
	bool CheckAmmo();
	bool CheckReload();
	
	UFUNCTION(BlueprintCallable)
	void Reloading();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire_Gun(FVector Location, FVector Direction);
	
	// RPM ( 연사 속도 )시간 끝내는함수 
	void HandleFireDelay();
	void AddDamage(float Add_RelicDamage,float Add_TotalDamage,float Critical);
	
	void BattleIn(const FHitResult& HitResult);
	void SelectParts(EPartsName parts);
	
	int32 GetMaxAmmoStat(){return MaxAmmo;}
	float GetReloadStat(){return ReloadTime;}
	void AddMaxAmmoStat(float AddAmmo){MaxAmmo +=AddAmmo;}
	void AddReloadStat(float AddReload);
	
	// Weapon Parts
	FGunParts Bullet;
	FGunParts Magazine;
	FGunParts Scope;
	FGunParts Handle;
	
protected:
	virtual void BeginPlay() override;
};
