#include "GunBase.h"
#include "Data/MasterSubsystem.h"
#include "Battle/BattleSubsystem.h"
#include "public/MonsterBase.h"
#include "DrawDebugHelpers.h"

bool AGunBase::HasAmmo()
{
	if (CanFire && !bReloadingCheck && CurrentAmmo > 0)
	{
		CanFire = false; // 사격 잠금

		// 부모의 타이머 핸들(TimerFireDelay)과 부모의 RoundsPerSecond를 사용해 타이머 등록
		// 명시적으로 1.0f 소수점 연산을 보장합니다.
		float FireInterval = 1.0f / FMath::Max(RoundsPerSecond, 0.1f);

		GetWorld()->GetTimerManager().SetTimer(
			TimerFireDelay,
			this,
			&AGunBase::GripFireDelay,
			FireInterval,
			false
		);
			
		return true;
	}
	return false;
}
bool AGunBase::CanReload()
{
	if (CurrentAmmo < MaxAmmo && bReloadingCheck == false )
	{
		bReloadingCheck = true;
		return true;
	}
	return false;
}
void AGunBase::Reloading()
{
	bReloadingCheck = false;
	CanFire = true;
	CurrentAmmo = MaxAmmo;

}
void AGunBase::FireGun(FVector Location, FVector Direction)
{
	float SafeSpread = FMath::Max(SpreadAngle, 0.0f);
	FVector SpreadDirection = FMath::VRandCone(Direction, FMath::DegreesToRadians(SafeSpread));
	FVector End = Location + (SpreadDirection * EffectiveRange);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);          
	Params.AddIgnoredActor(GetOwner());    

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Location,
		End,
		ECC_Visibility, 
		Params
	);
	FLinearColor LColor =bHit ? FLinearColor::Green : FLinearColor::Red; 
	
	DrawDebugLine(
		GetWorld(),
		Location,           // 시작 지점
		End,                // 끝 지점
		LColor.ToFColor(true),          // 색상
		false,              // 매 프레임마다 그릴지 여부 (고정된 라인은 false)
		2.0f,               // 유지 시간 (초 단위, 블루프린트의 For Duration과 대응)
		0,                  // 우선순위
		0.1f  //두께
	);
	if (bHit)
	{
		BattleIn(HitResult);
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,10.f,5,FColor::Green,false,3.f);
	}
}
void AGunBase::BattleIn(const FHitResult& HitResult)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(HitResult.GetActor());
	UBattleSubsystem* BattleSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UBattleSubsystem>() : nullptr;

	if (!Monster || !BattleSubsystem)
	{
		return;
	}

	// TODO: blsCritical 기능 추가
	BattleSubsystem->ExecuteDamageCalculation(
		GetOwner(), 
		Monster, 
		FinalDamage, 
		false, 
		CritMultiplier
	);
}
void AGunBase::PartsUpdate()
{
	
	SpreadAngle = FMath::Max(
		SpreadAngleDegrees -
		SpreadAngleDegrees * Scope.Level * Scope.Value,
		0.1f
	);
	ActiveRecoil = FMath::Max(
		Recoil -
		(Recoil * Grip.Level * Grip.Value) +
		AddedRecoil,
		0.1f
	);
	ActiveReload = FMath::Max(
		ReloadTime -
		ReloadTime * Magazine.Level * Magazine.Value +
		AddReloadTime,
		0.1f
	);
	AddDamage(0,0,0);
}

void AGunBase::LoadData(int32 GripLevel, int32 ScopeLevel, int32 MagazineLevel, int32 BulletLevel)
{
	Bullet.Level = BulletLevel;
	Magazine.Level =MagazineLevel;
	Scope.Level = ScopeLevel;
	Grip.Level = GripLevel;
}

void AGunBase::SaveData()
{
	UMasterSubsystem* MasterSubsystem = GetGameInstance()->GetSubsystem<UMasterSubsystem>();
	if (MasterSubsystem)
	{
		MasterSubsystem->OnSaveGun.Broadcast(Grip.Level, Scope.Level, Magazine.Level, Bullet.Level);
	}
}

void AGunBase::GripFireDelay()
{
	CanFire = true; 
	GetWorld()->GetTimerManager().ClearTimer(TimerFireDelay);
}
void AGunBase::AddDamage(float Add_RelicDamage, float Add_TotalDamage, float Critical)
{
	RelicBonus += Add_RelicDamage;
	TotalBonus += Add_TotalDamage / 100.0f;
	CritMultiplier += Critical;
	FinalDamage = (BaseDamage * (1 + Bullet.Value) + RelicBonus) * (1 + TotalBonus);
}
void AGunBase::SelectParts(EPartsName parts)
{
	if (parts == EPartsName::Bullet && Bullet.Level < MaxLevelParts)
	{
		++Bullet.Level;
	}
	else if (parts == EPartsName::Scope && Scope.Level < MaxLevelParts)
	{
		++Scope.Level;
	}
	else if (parts == EPartsName::Handle && Grip.Level < MaxLevelParts)
	{
		++Grip.Level;
	}
	else if (parts == EPartsName::Magazine && Magazine.Level < MaxLevelParts)
	{
		++Magazine.Level; 
	}
	PartsUpdate();
}
FGunParts AGunBase::GetPartsData(EPartsName PartsType) const
{
	switch (PartsType)
	{
	case EPartsName::Bullet:
		{
			return Bullet;
		}
	case EPartsName::Magazine:
		{
			return Magazine;
		}
	case EPartsName::Scope:
		{
			return Scope;
		}
	case EPartsName::Handle:
		{
			return Grip;
		}
	default:
		{
		return FGunParts();
		}
	}
}
void AGunBase::DecreaseReloadTimeStat(float AddReload)
{
	if (ActiveReload + AddReload >= 0.1f)
	{
		AddReloadTime += AddReload;
		ActiveReload = ReloadTime - ReloadTime* Magazine.Level *Magazine.Value +AddReloadTime;
	}
}

AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = false;
}
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
    
	CurrentAmmo = MaxAmmo;
	CanFire = true;
	bReloadingCheck = false;
	
	ActiveRecoil = Recoil; 
	ActiveReload = ReloadTime;
	SpreadAngle = SpreadAngleDegrees;
	
	InitializeParts();
	PartsUpdate();
}

void AGunBase::InitializeParts()
{
	Bullet.Name = "Bullet";
	Bullet.Value = LevelUpDamageValue;
	Bullet.Parts = EPartsName::Bullet;
	
	Magazine.Name = "Magazine";
	Magazine.Value = LevelUpReloadValue;
	Magazine.Parts = EPartsName::Magazine;
	
	Scope.Name = "Scope";
	Scope.Value = LevelUpScopeValue;  
	Scope.Parts = EPartsName::Scope;
	
	Grip.Name = "Handle";
	Grip.Value = LevelUpGripValue;
	Grip.Parts = EPartsName::Handle;
}
float AGunBase::GetCurrentRecoilPitch() const
{
	return FMath::Max(ActiveRecoil, 0.1f);
}
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanFire)
	{
		FireCooldownTimer -= DeltaTime;
		if (FireCooldownTimer <= 0.0f)
		{
			CanFire = true;
		}
	}
}
