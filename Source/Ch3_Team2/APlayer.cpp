
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
#include "Data/MasterSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Prop/HealTotem.h"


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
	ChildActors = CreateDefaultSubobject<UChildActorComponent>(TEXT("child"));
	ChildActors->SetupAttachment(GetMesh());
	
	PrimaryActorTick.bCanEverTick = true;
	
	MagnetComp = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetComp"));
	MagnetComp->SetupAttachment(RootComponent);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	CurrentTargetStructure = nullptr;
}

void AAPlayer::BeginPlay()
{
	Super::BeginPlay();
	PController = Cast<APlayerController>(GetController());
	LoadData(CurrentLevel,CurrentSkill,CurrentWeapon);
	
}


void AAPlayer::SwitchSkill(int32 Index)
{
	if (!MySkillInventory.IsValidIndex(Index) || !MySkillInventory[Index])
	{
		UE_LOG(LogTemp, Warning, TEXT("[SwitchSkill] 유효하지 않은 스킬 인덱스이거나 스킬이 비어있습니다."));
		return;
	}

	if (ActiveSkillComp)
	{
		ActiveSkillComp->SetComponentTickEnabled(false);
		ActiveSkillComp->UnregisterComponent();
	}

	ActiveSkillComp = MySkillInventory[Index];
	if (ActiveSkillComp)
	{
		ActiveSkillComp->RegisterComponent();
		if (ActiveSkillComp->CurrentSkillCoolTime > 0.0f)
		{
			ActiveSkillComp->SetComponentTickEnabled(true);
		}
		CurrentSkill = Index;

		UE_LOG(LogTemp, Log, TEXT("스킬 교체 완료: %s"), *ActiveSkillComp->GetName());
	}
}

void AAPlayer::SwitchWeapon(int32 Index)
{
	if (!WeaponBlueprintClasses.IsValidIndex(Index) || !WeaponBlueprintClasses[Index])
	{
		UE_LOG(LogTemp, Warning, TEXT("[SwitchWeapon] 유효하지 않은 인덱스이거나 무기가 비어있습니다."));
		return;
	}

	UClass* SpawnClass = WeaponBlueprintClasses[Index];
	if (!SpawnClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchWeapon] %d 번 인덱스의 블루프린트 에셋이 기어코 None입니다!"), Index);
		return;
	}
	// 1. 기존 무기 및 컴포넌트 내부 액터가 있다면 확실하게 제거
	if (EquipedGun)
	{
		EquipedGun->Destroy();
		EquipedGun = nullptr;
	}
	if (ChildActors)
	{
		ChildActors->SetChildActorClass(nullptr); 
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	EquipedGun = GetWorld()->SpawnActor<AGunBase>(
	   SpawnClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (EquipedGun && GetMesh() && ChildActors)
	{
		EquipedGun->SetActorEnableCollision(false);
		EquipedGun->SetOwner(this);
       
		// 3. 무기를 손 소켓에 딱 붙여줍니다.
		EquipedGun->AttachToComponent(
		   GetMesh(),
		   FAttachmentTransformRules::SnapToTargetIncludingScale,
		   TEXT("GripPoint")
		);
       
		EquipedGun->SetActorRelativeLocation(FVector::ZeroVector);
		EquipedGun->SetActorRelativeRotation(FRotator::ZeroRotator);

		ChildActors->SetChildActorClass(WeaponBlueprintClasses[Index]); // 클래스 타입 갱신

		if (ChildActors)
		{
			ChildActors->SetChildActorClass(SpawnClass);
		}
		EquipedGun->LoadData();
		CurrentWeapon = Index;

		UE_LOG(LogTemp, Log, TEXT("🔥 완벽한 융합 성공! C++ 자식 다형성 보장 및 컴포넌트 동기화 완료! 클래스: %s"), *EquipedGun->GetClass()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SwitchWeapon] ChildActor를 AGunBase 타입으로 형변환(Cast)하는데 실패했습니다."));
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
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		
		if (PController && bIsRecoveringRecoil)
		{
			// 현재 마우스 입력이 반영된 최종 회전값을 타겟으로 동기화
			UpRecoilTargetRotation = PController->GetControlRotation();
             
			// 만약 마우스를 아래로 크게 내려서 이미 원래 쐈던 위치보다 더 내려갔다면 복구를 종료합니다.
			bIsRecoveringRecoil = false;
		}
	}
}
void AAPlayer::Reload(const FInputActionValue& Value)
{
	
	if (EquipedGun &&EquipedGun->CanReload())
	{
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage, EquipedGun->GetReloadSpeed());
		}
	}
}
void AAPlayer::Shooting(const FInputActionValue& Value)
{
	if (!EquipedGun)
	{
		return;
	}
	
	if (EquipedGun && EquipedGun->HasAmmo())
	{
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

		if (AnimInstance && ShootMontage)
		{
			FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation(); // Get World Location 대응
			FVector CameraForward  = FirstPersonCameraComponent->GetForwardVector();     // Get Forward Vector 대응
			AnimInstance->Montage_Play(ShootMontage, 1.0f);
			EquipedGun->FireGun(CameraLocation,CameraForward);
			
				
			float FinalRecoilPitch = EquipedGun->GetCurrentRecoilPitch();
			RecoilRecoveryRotation += EquipedGun->GetCurrentRecoilPitch();
			
			if (PController)
			{
				if (!bIsRecoveringRecoil)
				{
					UpRecoilTargetRotation = PController->GetControlRotation();
					SetActorTickEnabled(true);

				}
				TargetRotation = PController->GetControlRotation();
				FRotator RecoilRot = TargetRotation	;
				RecoilRot.Pitch += FinalRecoilPitch; 
				PController->SetControlRotation(RecoilRot);
				bIsRecoveringRecoil = true;
					
				LevelUpStat();
			}
		}
	}
}
void AAPlayer::Interact(const FInputActionValue& Value)
{
	if (CurrentTargetStructure)
	{
		UE_LOG(LogTemp, Log, TEXT("[Player] %s 토템과 상호작용을 시작합니다."), *CurrentTargetStructure->GetName());
		CurrentTargetStructure->Interact(this);
        CurrentTargetStructure = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Player] 주변에 상호작용 가능한 토템이 없습니다."));
	}
}
void AAPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsRecoveringRecoil)
	{
		if (PController)
		{
			FRotator CurrentRotation = PController->GetControlRotation();
			FRotator NewRotation = FMath::RInterpTo(
				CurrentRotation,
				UpRecoilTargetRotation,
				DeltaTime,
				RecoilRecoverySpeed);
			
			float DeltaPitch = NewRotation.Pitch - CurrentRotation.Pitch;
			PController->SetControlRotation(NewRotation);
			RecoilRecoveryRotation -= DeltaPitch;
			
			if (FMath::IsNearlyEqual(
				CurrentRotation.Pitch,
				UpRecoilTargetRotation.Pitch,
				0.05f) && RecoilRecoveryRotation <= 0)
			{
				bIsRecoveringRecoil = false;
				SetActorTickEnabled(false);
				
			}
		}
	}

}
void AAPlayer::SkillInputKey(const FInputActionValue& Value)
{
	if (ActiveSkillComp)
	{
		ActiveSkillComp->ActiveSkill();
	}
}
void AAPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
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
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAPlayer::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAPlayer::Look);
		
		EnhancedInputComponent->BindAction(SkillActive, ETriggerEvent::Started, this, &AAPlayer::SkillInputKey);
		
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AAPlayer::Reload);
		
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AAPlayer::Shooting);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAPlayer::Interact);
		
	}
}
void AAPlayer::LoadData(int32 GetLevel, int32 GetSkill, int32 GetWeapon)
{
	CurrentLevel = GetLevel;
	CurrentWeapon = GetWeapon;
	CurrentSkill = GetSkill;
	
	for (USkillBaseComp* Skill : MySkillInventory)
	{
		if (Skill)
		{
			Skill->DestroyComponent();
		}
	}
	MySkillInventory.Empty();
	for (TSubclassOf<USkillBaseComp> SkillClass : SkillBlueprintClasses)
	{
		if (!SkillClass) continue;

		USkillBaseComp* NewSkill =
			NewObject<USkillBaseComp>(this, SkillClass);

		if (NewSkill)
		{
			NewSkill->RegisterComponent();
			NewSkill->SetComponentTickEnabled(false);
			MySkillInventory.Add(NewSkill);
		}
	}
	
	SwitchSkill(GetSkill);
	
	SwitchWeapon(GetWeapon);
	if (MagnetComp)
	{
		MagnetComp->SetSphereRadius(MagnetRadius);
	}
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	}
	AddMaxHp(0);
	AddPlayerSpeed(0);
}
void AAPlayer::SaveData()
{
	UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
	if (MasterSubsystem)
	{
		MasterSubsystem->OnSavePlayer.Broadcast(CurrentLevel,CurrentSkill,CurrentWeapon);
	}
}
void AAPlayer::AddCurrentHp(int32 Add_Hp)
{
	UE_LOG(LogTemp, Warning, TEXT("Current HP: %d , Add HP: %d"),CurrentHp, Add_Hp);
	if (CurrentHp + Add_Hp >= MaxHp)
	{
		CurrentHp = MaxHp;
		UE_LOG(LogTemp, Warning, TEXT("Current HP: %d"),CurrentHp);
	}
	else
	{
		CurrentHp += Add_Hp;
		UE_LOG(LogTemp, Warning, TEXT("Current HP: %d"),CurrentHp);
	}
}
void AAPlayer::AddMaxHp(int32 Add_Max_Hp)
{
	if (MaxHp + Add_Max_Hp >= 0)
	{
		RelicBonusHp += Add_Max_Hp;
		MaxHp = BaseMaxHp + CurrentLevel* MaxHPIncrease + RelicBonusHp;
		CurrentHp += Add_Max_Hp;
	}
}
void AAPlayer::AddPlayerSpeed(float Add_Speed)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{	
		if (MoveSpeed + Add_Speed > 0.0f )
		{
			RelicBonusSpeed += Add_Speed;
			MoveSpeed = BaseMoveSpeed + CurrentLevel*SpeedIncrease + RelicBonusSpeed; 
			MoveComp->MaxWalkSpeed = MoveSpeed;
			
		}
	}
}
void AAPlayer::AddExp(int32 Add_Exp)
{
	Exp += Add_Exp;
	if (Exp >= LevelUpExp &&CurrentLevel < MaxLevel)
	{
		Exp -= LevelUpExp;
		LevelUpStat();
	}
}
void AAPlayer::TotalDamageUpGrade(float AddRelicBonus, float TotalBonus,float Critical)
{
	if (EquipedGun)
	{
		EquipedGun->AddDamage(AddRelicBonus, TotalBonus, Critical);
	}
}
void AAPlayer::LevelUpStat()
{
	AddMaxHp(0);	
	CurrentHp = MaxHp;
	++CurrentLevel;
	LevelUpExp =FMath::RoundToInt32(BaseExp * FMath::Pow(BaseUpExp, CurrentLevel));
	AddPlayerSpeed(0);
}
void AAPlayer::DecreaseSkillCoolTime(float SkillCoolTime)
{
	if (ActiveSkillComp)
	{
		USkillBaseComp* Skill = Cast<USkillBaseComp>(ActiveSkillComp);
		Skill->	DecreaseTimeSkill(SkillCoolTime);
	}
}

void AAPlayer::UpgradeWeaponParts(EPartsName PartsType)
{
	if (EquipedGun)
	{
		EquipedGun->SelectParts(PartsType);
	}
}		
FGunParts AAPlayer::GetCurrentWeaponPartsData(EPartsName PartsType)
{
	if (EquipedGun)
	{
		return EquipedGun->GetPartsData(PartsType);
	}
	return FGunParts();
}	
float AAPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser)
{
	CurrentHp -= FMath::RoundToInt32(DamageAmount);
	CurrentHp = FMath::Clamp(CurrentHp, 0, MaxHp);
	
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
	// 부활  - bool 값 이 true인지 확인해서 
	// true 이면 부활 하도록 로그 수정하기
	//if ()
	// 사망 로그 호출
	UE_LOG(LogTemp, Error, TEXT("Player Dead"));
}
