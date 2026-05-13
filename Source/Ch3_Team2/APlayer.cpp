// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayer.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAPlayer::AAPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("child"));
	ChildActor->SetupAttachment(Mesh1P);
	
	ChildActor->AttachToComponent(
	Mesh1P,
	FAttachmentTransformRules::SnapToTargetIncludingScale,
	TEXT("GripPoint"));
	
	// Tick 함수 호출 false 거부 
	PrimaryActorTick.bCanEverTick = true;
	
	
	MagnetComp = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetComp"));
	MagnetComp->SetupAttachment(RootComponent);
	DropExpComp = CreateDefaultSubobject<USphereComponent>(TEXT("DropExpComp"));
	DropExpComp->SetupAttachment(MagnetComp);
	
	
	// 체력 
	MaxHp = 100;
	CurrentHp = MaxHp;
	// 속도 또는 좀프 관련
	MoveSpeed = 600.0f;
	JumpZVelocity = 420.0f;
	// 스킬 관련
	SkillCoolTime = 20.0f;
	//
	//ActiveSkilltime = 5.0f; - const화 해서 나중에 수정가능하게 할려면 const 제거 하고 할것
	CurrentSkillCoolTime= 0;
	// 언리얼 엔진에서는 cm단위이기 때문에 10m 는 1000cm이다.
	MagnetRadius = 1000.0f;
	// 경험치 및 레벨업 관련
	Exp = 0;
	LevelUpExp = 200;
	Level = 1;
	// 점프 높이 초기값 420
	GetCharacterMovement()->JumpZVelocity = 420.0f;
	
	// 자석 범위 설정
	MagnetComp->SetSphereRadius(MagnetRadius);
}

void AAPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 컨트롤로가 없으면 호출하지 않음.
	if (Controller != nullptr)
	{
		// add movement 
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

void AAPlayer::SkillInputKey(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(
	-1,	3.0f,FColor::Yellow,
	FString::Printf(TEXT("Skill Active: Cool %f"), CurrentSkillCoolTime));
	
	// 현제 스킬 쿨이 0.0f 이하면 호출 하도록
	if (CurrentSkillCoolTime <= 0.0f)
	{
		// 스킬 시전
		
		// 스킬 시전
		SkillTimeSlow();		
	}
}
void AAPlayer::SkillTimeSlow()
{
	// 전체적인 시간 느리게 하는 코드
	// 다만 
	CustomTimeDilation = 0.01f;
	this->CustomTimeDilation = 1.0f;
	// Tick 함수 활성화 
	SetActorTickEnabled(true);

	GetWorldTimerManager().SetTimer(
			SkillTimerHandle
			,this
			,&AAPlayer::SkillTimeNormal
			,ActiveSkilltime
			,false// 반복해서 함수를 호출해라
			);
	// 비 활성화 하는 TimeHandle 
	CurrentSkillCoolTime = SkillCoolTime;
}

void AAPlayer::SkillTimeNormal()
{
	// 사용한 TimeHandle은 초기화 해줄것
	GetWorldTimerManager().ClearTimer(SkillTimerHandle);
	
	//
	CurrentSkillCoolTime = SkillCoolTime;
	// 시간 정상화 
	CustomTimeDilation = 1.0f;
	this->CustomTimeDilation = 1.0f;
}

void AAPlayer::ActivateSkillCooldown(float DeltaTime)
{
	
	// 쿨타임이 0초 아래로 떨어지지 않았으면 쿨타임 계속해서 감소 
	if (CurrentSkillCoolTime > 0.0f)
		CurrentSkillCoolTime -= DeltaTime;
	else
		// 만약에 스킬  쿨 타임이 다 찼다면 Tick 함수 비활성화
		SetActorTickEnabled(false);
		
}

void AAPlayer::Tick(float DletaTime)
{
	GEngine->AddOnScreenDebugMessage(
	-1,	3.0f,FColor::Yellow,
	FString::Printf(TEXT("Skill Active: Cool %f"), CurrentSkillCoolTime));
	
	ActivateSkillCooldown(DletaTime);
	Super::Tick(DletaTime);
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
		
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



void AAPlayer::AddCurrentHp(int32 Add_Hp)
{
	// 체력회복시 회복된 량이 최대 체력보다 많으면 적으면
	if (CurrentHp + Add_Hp <= MaxHp)
		// 일단 체력추가
		CurrentHp += Add_Hp;
	else
		//많으면 최대 체력 까지만 추가
		CurrentHp = MaxHp;
}

void AAPlayer::AddMaxHp(int32 Add_Max_Hp)
{
	// 최대 체력 증가
	MaxHp += Add_Max_Hp;
	
	// 체력이 증가한 만큼 현제 체력도 증가
	CurrentHp += Add_Max_Hp;
}

void AAPlayer::AddExp(int32 Add_Exp)
{
	// 경험치 추가 
	Exp += Add_Exp;
	// Levelup경험치 초과시 
	// 레벨업 + 경험치 초기화
	// 만약  Lvelup 시 레벨업 경험치 증가시 따로 추가할 것
	 if (Exp >= LevelUpExp)
	 {
	 	// 최대 레벨이 16이기 때문에 일단 작성
	 	if (Level < 16)
	 	{
	 		Exp -= LevelUpExp;
	 		LevelupStat();
	 	}
	 }
}

void AAPlayer::LevelupStat()
{
	// 최대 체력
	MaxHp += 16;
	// 레벨업
	Level++;
	// 점프 값 증가
	JumpZVelocity += 8.0f;
	// 이동속도 증가
	MoveSpeed += 18.0f;
	// 점프 높이는 즉시 증가 
	GetCharacterMovement()->JumpZVelocity += 8.0f;
}




