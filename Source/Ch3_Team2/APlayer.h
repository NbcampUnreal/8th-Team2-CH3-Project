// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "APlayer.generated.h"

enum class EPartsName : uint8;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
struct FInputActionValue;
class AHealTotem;
class USkillBaseComp;
class AGunBase;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class CH3_TEAM2_API AAPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AHealTotem* CurrentTargetStructure;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;
	
	virtual void Tick(float DeltaTime) override;
	
	AAPlayer();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void Shooting(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

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
	

	// Stat
	const int32 MaxLevel = 16;
	const int32 MaxHPIncrease = 16;
	const float SpeedIncrease = 18.0f;
	const int32 BaseMaxHp = 100;
	const float BaseMoveSpeed = 1000.0f;
	const float BaseExp = 200.0f;
	const float BaseUpExp = 1.35f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHp = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentHp = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float JumpZVelocity = 420.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MagnetRadius = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Exp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 LevelUpExp = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed = 1000.0f;
	
	int32 RelicBonusHp = 0;
	float RelicBonusSpeed = 0.0f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* MagnetComp; 
	
	
	
	void LoadData(int32 GetLevel,int32 GetSkill, int32 GetWeapon);
	void SaveData();
	
	// Setter
	void SetRelicHp(int32 Set_Hp) { RelicBonusHp = Set_Hp;}
	void SetRelicSpeed(float Set_RelicSpeed){RelicBonusSpeed = Set_RelicSpeed;}
	void SetExp(int32 Set_Exp) {Exp = Set_Exp;}
	void SetLevel(int32 Set_Level) { CurrentLevel = Set_Level;}
	
	FORCEINLINE void SetCurrentStructure(class AHealTotem* NewStructure) 
	{ CurrentTargetStructure = NewStructure; }
	// Getter
	int32 GetCurrentHp() const {return CurrentHp;}
	int32 GetMapHp() const {return MaxHp;}
	float GetSpeed() const {return MoveSpeed;}
	int32 GetExp() const {return Exp;}
	int32 GetCurrentLevel() const {return CurrentLevel;}
	
	// OverDrive용으로 사용됩니다.
	float GetTotalSpeed() const {return MoveSpeed;}

	FORCEINLINE AHealTotem* GetCurrentStructure() const 
	{ return CurrentTargetStructure; }

	// Add
	void AddCurrentHp(int32 Add_Hp) ;
	void AddMaxHp(int32 Add_Max_Hp);
	void AddExp(int32 Add_Exp);
	void AddPlayerSpeed(float Add_Speed);
	
	void TotalDamageUpGrade(float AddRelicBonus, float TotalBonus ,float Critical);
	void DecreaseSkillCoolTime(float SkillCoolTime);
	void LevelUpStat();
	void OnDeath();
	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RecoilControll")
	APlayerController* PController;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RecoilControll")
	AGunBase* EquipedGun;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<AGunBase*> MyWeaponInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<AGunBase>> WeaponBlueprintClasses;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchWeapon(int32 Index);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Skill")
	USkillBaseComp* ActiveSkillComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<TSubclassOf<USkillBaseComp>> SkillBlueprintClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<USkillBaseComp*> MySkillInventory;
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SwitchSkill(int32 Index);
	
	UFUNCTION(BlueprintCallable, Category = "Player|Upgrade")
	void UpgradeWeaponParts(EPartsName PartsType);

	// UI에서 현재 파츠 정보를 화면에 띄우기 위해 데이터를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Player|Upgrade")
	FGunParts GetCurrentWeaponPartsData(EPartsName PartsType);

	float RecoilRecoveryRotation = 0;
	FRotator TargetRotation = FRotator::ZeroRotator;
	FRotator UpRecoilTargetRotation= FRotator::ZeroRotator;
	
	bool bIsRecoveringRecoil = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Recoil")
	float RecoilRecoverySpeed = 3.0f;
	
	virtual float TakeDamage(
		float DamageAmount
		,struct FDamageEvent const& DamageEvent
		,class AController* EventInstigator
		,AActor* DamageCauser
	) override;
};