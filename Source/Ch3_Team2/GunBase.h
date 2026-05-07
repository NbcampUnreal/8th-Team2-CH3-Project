// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GunBase.generated.h"

UCLASS()
class CH3_TEAM2_API AGunBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AGunBase();
	virtual void Fire() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload();
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	// NatveEnvent로 하이브리드 형으로 만들면 
	//_Implementaion 함수 이름 뒤에 작성해야하고 
	// 블루 프린트로 위임했기 때문에 코드 작성하면 안된다.
	bool CheckAmmo();

	//총알 업데이트
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAmmo();
	
	// 이부분은 아직 keep
	/*
	//총을 어떻게 쏠지
	UFUNCTION(BlueprintImplementableEvent)
	void ProcessFiring();

	//이펙트 실행
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEffects();
	 */
	
};
