// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "Battle/BattleSubsystem.h"
#include "public/MonsterBase.h"
#include "DrawDebugHelpers.h"

bool AGunBase::HasAmmo()
{
	if (CanFire && bReloadingCheck == false && CurrentAmmo > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerFireDelay
			,this
			,&AGunBase::HandleFireDelay
			,1.f / RoundsPerSecond
			,false
		);
		CanFire = false;
		
		return true;
	}
	return false;
}
bool AGunBase::CanReload()
{
	// 총알이 가득 차있을 떄 
	if (CurrentAmmo < MaxAmmo && bReloadingCheck == false )
	{
		bReloadingCheck = true;
		return true;
	}
	return false;
}
void AGunBase::Reloading()
{
	// 재장전 되었을 떄 
	bReloadingCheck = false;
	CurrentAmmo = MaxAmmo;
}
void AGunBase::FireGun(FVector Location, FVector Direction)
{
	FVector SpreadDirection = FMath::VRandCone(Direction, FMath::DegreesToRadians(SpreadAngleDegrees ));
	FVector End = Location + (SpreadDirection * EffectiveRange);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);           // 자기 자신 제외
	Params.AddIgnoredActor(GetOwner());    // 소유자(예: 캐릭터) 제외

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Location,
		End,
		ECC_Visibility, // 블루프린트의 Visibility 채널
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
void AGunBase::HandleFireDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerFireDelay);
	CanFire = true;
}

void AGunBase::AddDamage(float Add_RelicDamage,float Add_TotalDamage,float Critical)
{
	// 성유물 기본 공격력 증가
	RelicBonus += Add_RelicDamage;
	TotalBonus += Add_TotalDamage;
	CritMultiplier += Critical;
	FinalDamage = (BaseDamage * (1 + Bullet.Value )+ RelicBonus ) * TotalBonus;
}
void AGunBase::SelectParts(EPartsName parts)
{
	if (parts == EPartsName::Bullet && Bullet.Level < MaxLevelParts)
	{
		++Bullet.Level;
		Bullet.Value += LevelUpDamageValue;
		AddDamage(0,0,0);
	}
	else if (parts == EPartsName::Magazine && Magazine.Level < MaxLevelParts)
	{
		++Scope.Level;
		SpreadAngleDegrees -= Scope.Value;
	}
	else if (parts == EPartsName::Scope && Scope.Level < MaxLevelParts)
	{
		++Handle.Level;
		Recoil -= Handle.Value; 
	}
	else if (parts == EPartsName::Handle && Handle.Level < MaxLevelParts)
	{
		++Magazine.Level;
		ReloadTime -= Magazine.Value;
	}
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
			return Handle;
		}
	default:
		{
			
		return FGunParts();
		}
	}
}
void AGunBase::DegreaseReloadTimeStat(float AddReload)
{
	if (ReloadTime + AddReload >= 0.1f)
	{
		ReloadTime += AddReload;
	}
}
void AGunBase::InitializeParts()
{
	Bullet.Name = "Bullet";
	Bullet.Level = 0;
	Bullet.Value = 0;
	Bullet.Parts = EPartsName::Bullet;
	
	Magazine.Name = "Magazine";
	Magazine.Level = 1;
	Magazine.Value = ReloadTime*LevelUpReloadValue;
	Magazine.Parts = EPartsName::Magazine;
	
	Scope.Name = "Scope";
	Scope.Level = 0;  
	Scope.Value = SpreadAngleDegrees*LevelUpScopeValue;  
	Scope.Parts = EPartsName::Scope;
	
	Handle.Name = "Handle";
	Handle.Level = 0;
	Handle.Value = Recoil* LevelUpHandleValue;
	Handle.Parts = EPartsName::Handle;
}
float AGunBase::GetCurrentRecoilPitch() const
{
	// 핸들 파츠의 Value가 반동 감소율(예: 0.2 -> 20% 감소)이라면 아래와 같이 계산
	// 초기 Value가 0이므로, (1.0f - Handle.Value) 형태로 보정합니다.
	float RecoilModifier = FMath::Clamp(1.0f - Handle.Value, 0.1f, 1.0f);
	return Recoil * RecoilModifier;
}

void AGunBase::BeginPlay()
{
	InitializeParts();
	AddDamage(0,0,0);
	
}
