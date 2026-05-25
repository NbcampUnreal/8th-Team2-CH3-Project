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
	virtual void BeginPlay() override;
	void InitializeParts();
	bool CanShoot();
	bool CanReload();
	
	UFUNCTION(BlueprintCallable)
	void Reloading();
	virtual void FireGun(FVector Location, FVector Direction);
	
	
	void ResetFireCooldown();
	void AddDamage(float Add_RelicDamage,float Add_TotalDamage,float Critical);
	
		
	void BattleIn(const FHitResult& HitResult);
	
	
	void AddAmmo(float AddAmmo){MaxAmmo +=AddAmmo;}
	void AddCritical(float Critical){CritMultiplier += Critical;}
	
	void DecreaseReloadTimeStat(float AddReload);

	void PartsUpdate();
	void LoadData(int32 GripLevel, int32 ScopeLevel, int32 MagazineLevel, int32 BulletLevel);
	void SaveData();
	//set
	void SetBulletLevel(int32 Level){Bullet.Level = Level;}
	void SetMagazineLevel(int32 Level){Magazine.Level = Level;}
	void SetScopeLevel(int32 Level){Scope.Level = Level;}
	void SetGripLevel(int32 Level){Grip.Level = Level;}
	
	void SetRelicBonus(float SetBonus){RelicBonus = SetBonus;}
	void SetTotalBonus(float SetBonus){TotalBonus = SetBonus;}
	void SetCritMultiplier(float SetBonus){CritMultiplier = SetBonus;}
	
	float GetRelicBonus()const{return RelicBonus;}
	float GetTotalBonus()const{return TotalBonus;}
	float GetCritMultiplier()const{return CritMultiplier;}
		
	//get
	int32 GetCurrentAmmo()const{return CurrentAmmo;}
	int32 GetMaxAmmo()const{return MaxAmmo;}
	float GetReloadSpeed()const{return ReloadTime;}
	
	int32 GetGripLevel()const{return Grip.Level;}
	int32 GetScopeLevel()const{return Scope.Level;}
	int32 GetMagazineLevel()const{return Magazine.Level;}
	int32 GetBulletLevel()const{return Bullet.Level;}
	
	//const
	const int32 MaxLevelParts = 4;
	const float LevelUpDamageValue = 0.25f;
	const float LevelUpReloadValue = 0.15f;
	const float LevelUpScopeValue = 0.2f;
	const float LevelUpGripValue =0.2f;
	
	// 집탄률
	float SpreadAngle = 0;
	float ActiveRecoil = 0;
	float ActiveReload = 0;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 WeaponIndex = 0;                           
	float AddReloadTime = 0;
	float AddedRecoil = 0;
	
	bool bTriggerReleased = true; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Bullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Magazine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Scope;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Grip;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Parts")
	void SelectParts(EPartsName parts);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Parts")
	FGunParts GetPartsData(EPartsName PartsType) const;
	
	float FireCooldownTimer = 0.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Recoil")
	float GetCurrentRecoilPitch() const;
	
	bool AutoMatickCheck()const {return bIsAutomatic;}
};
