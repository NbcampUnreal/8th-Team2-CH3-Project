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

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class CH3_TEAM2_API AAPlayer : public ACharacter
{
	GENERATED_BODY()

	// 1인칭 
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	//1인칭용 카메라
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	// 맵핑
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	/** Jump to Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillActive;
	
public:
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// 스킬입력 키
	void SkillInputKey(const FInputActionValue& Value);
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	virtual void Tick(float DletaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Child")
	UChildActorComponent* ChildActor;
	
//public:
	// 생성자
	AAPlayer();
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	// 현제 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentHp;
	
	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHp;
	
	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;
	
	// 점프 z 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float JumpZVelocity;
	
	// 범위 안에 들어온 Actor(  경험치 ) 를 끌어오는 comp
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* MagnetComp; 
	
	// 하나더 어느정도 범위 안으로 들어왔으면 overlap 판정하여 경험치 획득 판정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExpDrop")
	USphereComponent* DropExpComp;
	
	// 경험치 드랍 자석 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MagnetRadius;
	
	// 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Exp;
	// 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 LevelUpExp;
	
	// 현제 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Level;
	
public:
	// ---- stat -----
	// 받는 값
	void SetHp(int32 Set_Hp) { MaxHp = Set_Hp;}
	// 현제 체력 회복
	void AddCurrentHp(int32 Add_Hp) ;
	// 최대 체력 증가
	void AddMaxHp(int32 Add_Max_Hp);
	
	// return 값
	// 체력 return 
	const int32 GetHp() {return CurrentHp;}
	
	// 경험치 추가 함수 
	void AddExp(int32 Add_Exp);
	
	void LevelupStat();
	
	//------- 스킬 라인 ------
	// 액티브 스킬 사용후 재사용 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCoolTime;

	// 현제  스킬 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CurrentSkillCoolTime;
	bool SkillActiveCheck; //스킬 사용여부 SkillInputKey 에 추가해서 중복 스킬 잠가버리기
	//스킬 효과가 유지되는 지속 시간
	const float ActiveSkilltime =5.0f;
	
	// 스킬 시전 지속시간 스킬handle
	FTimerHandle SkillTimerHandle;
	// 속도 감소 스킬 활성화
	void SkillTimeSlow();
	
	//  감소된 속도 정상화 
	void SkillTimeNormal();
	
	void ActivateSkillCooldown(float DeltaTime);
};
