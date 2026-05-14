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
	
	// 장전 모션 끝
	void Reload_End();

	// 무기 스텟 초기화 
	// 매개 변수 설정
	virtual void Stats_Initialize();
	
//protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// NatveEnvent로 하이브리드 형으로 만들면 
	//_Implementaion 함수 이름 뒤에 작성해야하고 
	// 블루 프린트로 위임했기 때문에 코드 작성하면 안된다.
	bool CheckAmmo();
	
	// 블루프린트의 "fireGun" Custom Event에 해당하는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire_Gun(FVector Location, FVector Direction);

	// RPM ( 연사 속도 )시간 끝내는함수 
	void HandleFireDelay();
	// 공격력 추가
	void AddDamage(float Add_RelicDamage,float Add_TotalDamage,float Critical);
	
	// 최종 데미지 량
	// 성유물 
	float GetFinalDamage() { return FinalDamage;}
	// 최종 데미지 최신화
	void FinalDamageCheck (){ }
	// 파츠 선택
	void SelectParts(EPartsName parts);
	// 무기 Parts
	// 총알
	FGunParts Bullet;
	
	// 탄창
	FGunParts Magazine;
	
	// 스코프
	FGunParts Scope;
	
	// 손잡이
	FGunParts Handle;
	
	// 파츠 정보 초기화 
	void InitializeParts();
	
	virtual void BeginPlay() override;
	
};
