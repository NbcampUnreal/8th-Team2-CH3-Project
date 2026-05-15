// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayer.h"

#include "SkillBaseComp.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GunBase.h"
#include "InputActionValue.h"
#include "SkillBaseComp.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAPlayer::AAPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("child"));
	ChildActor->SetupAttachment(Mesh1P);
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	MagnetComp = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetComp"));
	MagnetComp->SetupAttachment(RootComponent);
	DropExpComp = CreateDefaultSubobject<USphereComponent>(TEXT("DropExpComp"));
	DropExpComp->SetupAttachment(MagnetComp);
	
	GetCharacterMovement()->JumpZVelocity = 420.0f;
	
}

void AAPlayer::PlayerInit()
{
	// Attach Weapon
	ChildActor->AttachToComponent(
		Mesh1P,
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("GripPoint")
	);
}

void AAPlayer::StatInitialization()
{
	MaxHp = 100;
	CurrentHp = MaxHp;
	
	MoveSpeed = 600.0f;
	JumpZVelocity = 420.0f;
	
	MagnetRadius = 1000.0f;
	MagnetComp->SetSphereRadius(MagnetRadius);
	
	Exp = 0;
	LevelUpExp = 200;
	Level = 1;
}

void AAPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerInit();
	
    StatInitialization();
	
	// NewObject<타입>(Outer, Class)
	// Skil component Object화 
	SkillInstance = NewObject<UObject>(this, SkillComp);

	// 만약 인터페이스나 특정 클래스로 형변환해서 쓰고 싶다면:
	// USkillBase* Skill = Cast<USkillBase>(SkillInstance);
}

void AAPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 컨트롤로가 없으면 호출하지 않음.
	if (Controller != nullptr)
	{
		// 입력방향으로 이동
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * MoveSpeed);
		AddMovementInput(GetActorRightVector(), MovementVector.X * MoveSpeed);
	}
}
void AAPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAPlayer::Reload(const FInputActionValue& Value)
{
	AGunBase* Gun = Cast<AGunBase>(ChildActor);
	Gun->Reload();
}
void AAPlayer::SkillInputKey(const FInputActionValue& Value)
{
	if (SkillInstance)
	{
		USkillBaseComp* Skill = Cast<USkillBaseComp>(SkillInstance);
		if (!Skill->IsRegistered())
		{
			Skill->RegisterComponent();
		}
		else
		{
			Skill->ActiveCheck();
		}
	}
}
void AAPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
void AAPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAPlayer::Look);
		
		//Skill Activing
		EnhancedInputComponent->BindAction(SkillActive, ETriggerEvent::Started, this, &AAPlayer::SkillInputKey);
		
		//Reload Activing
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AAPlayer::SkillInputKey);
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAPlayer::AddCurrentHp(int32 Add_Hp)
{
	if (CurrentHp + Add_Hp <= MaxHp)
	{
		CurrentHp += Add_Hp;
	}
	else
	{
		CurrentHp = MaxHp;
	}
}

void AAPlayer::AddMaxHp(int32 Add_Max_Hp)
{
	MaxHp += Add_Max_Hp;
	CurrentHp += Add_Max_Hp;
}

void AAPlayer::AddPlayerSpeed(float Add_Speed)
{
	MoveSpeed += Add_Speed;
}

void AAPlayer::TotalDamageUpGrade(float AddRelicBonus, float TotalBonus,float Critical)
{
	if (ChildActor)
	{
		AGunBase* Gun = Cast<AGunBase>(ChildActor);
		if (!Gun) return;
		Gun->AddDamage(AddRelicBonus,TotalBonus,Critical);
	}
}

void AAPlayer::AddExp(int32 Add_Exp)
{
	Exp += Add_Exp;
	// Levelup경험치 초과시 
	// 레벨업 + 경험치 초기화
	// 만약  Lvelup 시 레벨업 경험치 증가시 따로 추가할 것
	 if (Exp >= LevelUpExp)
	 {
	 	if (Level < 16)
	 	{
	 		Exp -= LevelUpExp;
	 		LevelUpStat();
	 	}
	 }
}
void AAPlayer::LevelUpStat()
{
	MaxHp += 16;
	Level++;
	JumpZVelocity += 8.0f;
	MoveSpeed += 18.0f;
	GetCharacterMovement()->JumpZVelocity += 8.0f;
}

void AAPlayer::DegreaseSkillCoolTime(float SkillCoolTime)
{
	if (SkillInstance)
	{
		USkillBaseComp* Skill = Cast<USkillBaseComp>(SkillInstance);
		Skill->SkiilDegreaseTime(SkillCoolTime);
	}
}

float AAPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser)
{
	CurrentHp -= FMath::RoundToInt32(DamageAmount);
	CurrentHp = FMath::Clamp(CurrentHp, 0.f, MaxHp);
	
	UE_LOG(LogTemp, Warning,
		TEXT("Player Hit! Damage: %f CurrentHP: %d"),
		DamageAmount,
		CurrentHp
	);
	
	/*
	 Super::TakeDamage(
		DamageAmount //  데미지
		,DamageEvent // 이벤트
		,EventInstigator // Player를 공격한 actor ( 몬스터 ) controller 
		,DamageCauser); // 때린 Actor 데이터 
	 */
	
	if (CurrentHp <= 0.f)
	{
		OnDeath();
	}

	return DamageAmount;
}

void AAPlayer::OnDeath()
{
	// 사망 로그 호출
	UE_LOG(LogTemp, Error, TEXT("Player Dead"));
}

