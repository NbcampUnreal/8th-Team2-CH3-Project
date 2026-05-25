#include "GunBase.h"
#include "Data/MasterSubsystem.h"
#include "Battle/BattleSubsystem.h"
#include "public/MonsterBase.h"
#include "DrawDebugHelpers.h"

bool AGunBase::CanShoot()
{
	if (bReloadingCheck)
	{
		return false;
	}
	
	if (CurrentAmmo <= 0)
	{
		return false;
	}

	if (!CanFire)
	{
		return false;
	}
	if (!bIsAutomatic && !bTriggerReleased)
	{
		return false;
	}
	CanFire = false;

	bTriggerReleased = false;
	
	float FireInterval =
		1.0f / FMath::Max(RoundsPerSecond, 0.1f);

	GetWorld()->GetTimerManager().SetTimer(
		TimerFireDelay,
		this,
		&AGunBase::ResetFireCooldown,
		FireInterval,
		false
	);
	return true;
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
		Location,           
		End,                
		LColor.ToFColor(true),
		false,              
		2.0f,               
		0,                  
		0.1f  
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

void AGunBase::ResetFireCooldown()
{
	CanFire = true; 
	GetWorld()->GetTimerManager().ClearTimer(TimerFireDelay);
}
void AGunBase::AddDamage(float Add_RelicDamage, float Add_TotalDamage, float Critical)
{
	RelicBonus += Add_RelicDamage;
	TotalBonus += Add_TotalDamage / 100.0f;
	CritMultiplier += Critical;
	float BulletBonusDamage =BaseDamage * Bullet.Level * Bullet.Value;
	FinalDamage =(BaseDamage +BulletBonusDamage +RelicBonus)* (1.0f + TotalBonus);
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