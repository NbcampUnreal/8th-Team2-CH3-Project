
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
#include "Kismet/GameplayStatics.h"
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
	}
}

void AAPlayer::SwitchWeapon(int32 Index)
{
	// 1. 현재 들고 있는 무기 숨기기 및 비활성화
	if (EquipedGun)
	{
		EquipedGun->SetActorHiddenInGame(true);
		EquipedGun->SetActorEnableCollision(false);
		EquipedGun->SetOwner(nullptr);
		EquipedGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	// 2. 월드에 배치된 모든 AGunBase 액터 찾기
	TArray<AActor*> FoundGuns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGunBase::StaticClass(), FoundGuns);

	AGunBase* TargetGun = nullptr;
	for (AActor* Actor : FoundGuns)
	{
		AGunBase* Gun = Cast<AGunBase>(Actor);
		if (Gun)
		{
			Gun->SetActorHiddenInGame(true);
			Gun->SetActorEnableCollision(false);
			Gun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            
			if (Gun->WeaponIndex == Index)
			{
				TargetGun = Gun;
			}
		}
	}

	// 3. 찾은 무기 장착
	if (TargetGun)
	{
		EquipedGun = TargetGun;
        
		// 무기 활성화
		EquipedGun->SetActorHiddenInGame(false);
		EquipedGun->SetActorEnableCollision(true);
        EquipedGun->SetOwner(this);
		// 손 소켓에 부착
		EquipedGun->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("GripPoint")
		);
        
		EquipedGun->SetActorRelativeLocation(FVector::ZeroVector);
		EquipedGun->SetActorRelativeRotation(FRotator::ZeroRotator);
		EquipedGun->PartsUpdate();
		CurrentWeapon = Index;
	}
}
void AAPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
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
			UpRecoilTargetRotation = PController->GetControlRotation();
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
			FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation(); 
			FVector CameraForward  = FirstPersonCameraComponent->GetForwardVector();     
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
			}
		}
	}
}
void AAPlayer::Interact(const FInputActionValue& Value)
{
	if (CurrentTargetStructure)
	{
		CurrentTargetStructure->Interact(this);
        CurrentTargetStructure = nullptr;
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
	if (CurrentHp + Add_Hp >= MaxHp)
	{
		CurrentHp = MaxHp;
	}
	else
	{
		CurrentHp += Add_Hp;
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
	if (CurrentHp <= 0.f)
	{
		OnDeath();
	}
	return DamageAmount;
}
void AAPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SaveData();
	Super::EndPlay(EndPlayReason);
}

void AAPlayer::OnDeath()
{
	// 부활  - bool 값 이 true인지 확인해서 
	// true 이면 부활 하도록 로그 수정하기
	//if ()
	// 사망 로그 호출
}
