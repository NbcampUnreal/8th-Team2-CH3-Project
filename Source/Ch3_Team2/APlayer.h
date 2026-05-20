// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "APlayer.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
struct FInputActionValue;

class USkillBaseComp;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class CH3_TEAM2_API AAPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// 1인칭 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	// Mapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillActive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;
	
	virtual void Tick(float DeltaTime) override;
	
public:
	AAPlayer();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void Shooting(const FInputActionValue& Value);
	
	void SkillInputKey(const FInputActionValue& Value);
	
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// 에디터 디테일 패널에서 몽타주 에셋을 넣을 수 있는 슬롯을 만듭니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ShootMontage;
	
	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Child")
	UChildActorComponent* ChildActor;

	virtual void BeginPlay() override;
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	FRotator TargetRotation;     // 복구해야 할 원본 조준선 저장용

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Pitch = 2.0f;          // 총 쏠 때 위로 튕길 각도 (도 단위)

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Rebound = 10.0f;       // 원래대로 돌아오는 속도
	
	// Stat
	
	const int32 MaxLevel = 16;
	const int32 MaxHPIncrease = 16;
	const float SpeedIncrease = 18.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHp = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentHp = MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float JumpZVelocity = 420.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MagnetRadius = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Exp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 LevelUpExp = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* MagnetComp; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* DropExpComp;
	
	
	
	// Setter
	void SetHp(int32 Set_Hp) { MaxHp = Set_Hp;}
	
	// Getter
	int32 GetCurrentHp() const {return CurrentHp;}
	int32 GetMapHp() const {return MaxHp;}
	float GetSpeed() const {return MoveSpeed;}
	
	// Add
	void AddCurrentHp(int32 Add_Hp) ;
	void AddMaxHp(int32 Add_Max_Hp);
	void AddExp(int32 Add_Exp);
	void AddPlayerSpeed(float Add_Speed);
	
	void TotalDamageUpGrade(float AddRelicBonus, float TotalBonus ,float Critical);
	void DegreaseSkillCoolTime(float SkillCoolTime);
	void LevelUpStat();
	void OnDeath();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Skill")
	TSubclassOf<USkillBaseComp> SkillComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Skill")
	UObject* SkillInstance;
	
	// UI에서 파츠 레벨업 버튼을 누르면 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "Player|Upgrade")
	void UpgradeWeaponParts(EPartsName PartsType);

	// UI에서 현재 파츠 정보를 화면에 띄우기 위해 데이터를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Player|Upgrade")
	FGunParts GetCurrentWeaponPartsData(EPartsName PartsType);
	
	virtual float TakeDamage(
		float DamageAmount
		,struct FDamageEvent const& DamageEvent
		,class AController* EventInstigator
		,AActor* DamageCauser
	) override;
};
