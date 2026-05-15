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

	// 1인칭 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;
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
	
public:
	AAPlayer();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	
	void SkillInputKey(const FInputActionValue& Value);
	
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Child")
	UChildActorComponent* ChildActor;

	virtual void BeginPlay() override;
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	// Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float JumpZVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* MagnetComp; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* DropExpComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MagnetRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 LevelUpExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Level;
	
	void PlayerInit();
	void StatInitialization();
	
	// Setter
	void SetHp(int32 Set_Hp) { MaxHp = Set_Hp;}
	
	// Getter
	int32 GetCurrentHp() const {return CurrentHp;}
	int32 GetMapHp() const {return MaxHp;}
	
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
	
	virtual float TakeDamage(
		float DamageAmount
		,struct FDamageEvent const& DamageEvent
		,class AController* EventInstigator
		,AActor* DamageCauser
	) override;
};
