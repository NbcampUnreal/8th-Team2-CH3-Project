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
	
	int32 GetCurrentAmmo(){return CurrentAmmo;}
	int32 GetMaxAmmo(){return MaxAmmo;}
	void AddAmmo(float AddAmmo){MaxAmmo +=AddAmmo;}
	
	float GetReloadSpeed(){return ReloadTime;}
	void AddReloadStat(float AddReload);
	
	void AddCritical(float Critical){CritMultiplier += Critical;}
	
	
	// Weapon Parts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Bullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Magazine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Scope;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Parts")
	FGunParts Handle;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Parts")
	void SelectParts(EPartsName parts);

	// [추가] UI에서 특정 파츠의 현재 정보(이름, 레벨, 수치)를 읽어갈 수 있는 Getter 함수
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Parts")
	FGunParts GetPartsData(EPartsName PartsType) const;
	
	const int32 MaxLevelParts = 4;
	const float LevelUpDamageValue = 0.25f;
	const float LevelUpReloadValue = 0.15f;
	const float LevelUpScopeValue = 0.2f;
	const float LevelUpHandleValue =0.2f;
	
	// [추가] 핸들 파츠(반동 감소) 스탯이 반영된 최종 반동 값을 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon|Recoil")
	float GetCurrentRecoilPitch() const;
	
protected:
	virtual void BeginPlay() override;
};
