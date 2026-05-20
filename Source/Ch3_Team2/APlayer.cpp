// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayer.h"

#include <rapidjson/internal/ieee754.h>

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

	GetMesh()->AddRelativeLocation(FVector(10.0f,0,0));
	GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("child"));
	ChildActor->SetupAttachment(GetMesh());
	
	PrimaryActorTick.bCanEverTick = true;
	
	MagnetComp = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetComp"));
	MagnetComp->SetupAttachment(RootComponent);
	DropExpComp = CreateDefaultSubobject<USphereComponent>(TEXT("DropExpComp"));
	DropExpComp->SetupAttachment(MagnetComp);
	
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void AAPlayer::BeginPlay()
{
	
	Super::BeginPlay();
	MagnetComp->SetSphereRadius(MagnetRadius);
	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	// [추가] 게임 시작 직후, 기본 내장 Mesh의 PistolSocket 위치에 무기를 강제로 붙입니다.
	if (ChildActor && GetMesh())
	{
		ChildActor->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("GripPoint") // 사용할 소켓 이름
		);
	}
	
	if (SkillComp)
	{
		SkillInstance = NewObject<USkillBaseComp>(this, SkillComp);
		if (USkillBaseComp* Skill = Cast<USkillBaseComp>(SkillInstance))
		{
			Skill->RegisterComponent();
		}
	}
}
void AAPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	// 컨트롤로가 없으면 호출하지 않음.
	if (Controller != nullptr)
	{
		// 입력방향으로 이동
		AddMovementInput(GetActorForwardVector(), MovementVector.Y );
		AddMovementInput(GetActorRightVector(), MovementVector.X );
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
	if (ChildActor)
	{
		AGunBase* Gun = Cast<AGunBase>(ChildActor->GetChildActor());
		if (Gun->CheckReload())
		{
			// 1. 현재 상속받아 사용 중인 기본 Mesh에서 애님 인스턴스를 추출합니다.
			UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
			// 2. 애님 인스턴스와 우리가 에디터에서 등록할 몽타주 에셋이 모두 안전하게 존재할 때만 실행합니다.
			if (AnimInstance && ReloadMontage)
			{
				if (!AnimInstance->Montage_IsPlaying(ReloadMontage))
				{
					// 3. 몽타주를 재생합니다. (인자값: 몽타주에셋, 재생속도배율)
					AnimInstance->Montage_Play(ReloadMontage, Gun->GetReloadSpeed());
				}
			}
		}
	}
}
void AAPlayer::Shooting(const FInputActionValue& Value)
{
	if (ChildActor)
	{
		AGunBase* Gun = Cast<AGunBase>(ChildActor->GetChildActor());
		if (Gun->CheckAmmo())
		{
			UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
			if (AnimInstance && ShootMontage)
			{
				if (!AnimInstance->Montage_IsPlaying(ShootMontage))
				{
					// 2. [블루프린트 완벽 재현] 카메라의 위치와 정면 방향 벡터를 구합니다.
					FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation(); // Get World Location 대응
					FVector CameraForward  = FirstPersonCameraComponent->GetForwardVector();     // Get Forward Vector 대응
					// 3. 몽타주를 재생합니다. (인자값: 몽타주에셋, 재생속도배율)
					AnimInstance->Montage_Play(ShootMontage, 1.0f);
					Gun->Fire_Gun(CameraLocation,CameraForward);
					// 카메라 반동
					if (APlayerController* PC = Cast<APlayerController>(GetController()))
					{
						// 1. 총을 쏘기 직전, 현재 정상 조준선을 목표(복구 지점)로 저장합니다.
						TargetRotation = PC->GetControlRotation();
						
						// 2. 현재 시선을 Pitch만큼 강제로 위로 튕깁니다. (-값이 위 방향)
						FRotator RecoilRot = TargetRotation;
						RecoilRot.Pitch += Pitch;
						
						PC->SetControlRotation(RecoilRot);
					}
				}
			}
		}
	}
}
void AAPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AAPlayer::SkillInputKey(const FInputActionValue& Value)
{
	if (USkillBaseComp* Skill = Cast<USkillBaseComp>(SkillInstance))
	{
		Skill->ActiveSkill();
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
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AAPlayer::Reload);
		
		// Shoot Actiiving
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AAPlayer::Shooting);
		
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
	// 캐릭터 무브먼트 컴포넌트를 가져옵니다.
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		if (MoveSpeed + Add_Speed > 0.0f )
		{
			MoveSpeed += Add_Speed;
			// 실제 최대 이동 속도를 더해줍니다.
			MoveComp->MaxWalkSpeed = MoveSpeed;
			// 내 스탯 변수에도 동기화하고 싶다면 같이 업데이트
			
		}
	}
}
void AAPlayer::TotalDamageUpGrade(float AddRelicBonus, float TotalBonus,float Critical)
{
	if (ChildActor)
	{
		AGunBase* Gun = Cast<AGunBase>(ChildActor->GetChildActor());
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
	 	if (Level < MaxLevel)
	 	{
	 		Exp -= LevelUpExp;
	 		LevelUpStat();
	 	}
	 }
}
void AAPlayer::LevelUpStat()
{
	MaxHp += MaxHPIncrease;
	++Level;
	// 여기 매직넘버는 추후 수정 예정
	LevelUpExp =static_cast<int>(200 * pow(1.35, Level));
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveSpeed += SpeedIncrease;
		MoveComp->MaxWalkSpeed = MoveSpeed; // ★ 실제 걷기 속도에 반영 필수!
	}
}
void AAPlayer::DegreaseSkillCoolTime(float SkillCoolTime)
{
	if (SkillInstance)
	{
		USkillBaseComp* Skill = Cast<USkillBaseComp>(SkillInstance);
		Skill->DecreaseTimeSkill(SkillCoolTime);
	}
}

void AAPlayer::UpgradeWeaponParts(EPartsName PartsType)
{
	if (ChildActor)
	{
		// 1. 현재 들고 있는 무기 액터를 가져와 AGunBase로 형변환
		AGunBase* Gun = Cast<AGunBase>(ChildActor->GetChildActor());
		if (Gun)
		{
			// 2. 무기의 파츠 업그레이드 함수 실행
			Gun->SelectParts(PartsType);
		}
	}
}

FGunParts AAPlayer::GetCurrentWeaponPartsData(EPartsName PartsType)
{
	if (ChildActor)
	{
		AGunBase* Gun = Cast<AGunBase>(ChildActor->GetChildActor());
		if (Gun)
		{
			// 무기가 가진 파츠 정보를 UI단으로 그대로 토스합니다.
			return Gun->GetPartsData(PartsType);
		}
	}
    
	// 무기가 없다면 텅 빈 구조체 반환
	return FGunParts();
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

