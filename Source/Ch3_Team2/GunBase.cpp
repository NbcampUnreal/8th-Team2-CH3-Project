// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "Battle/BattleSubsystem.h"
#include "public/MonsterBase.h"

bool AGunBase::CheckAmmo_Implementation()
{
	if (CanFire)
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
void AGunBase::Reload_Implementation()
{
	// 애니메이션이 끝날 때 호출이 되어야 좋은함수
	CurrentAmmo = MaxAmmo;
	
	GetWorld()->GetTimerManager().SetTimer(
		TimeReloadDelay
		,this
		,&AGunBase::Reload_End
		,ReloadTime
		,false
	);
}

void AGunBase::Reload_End()
{
	
}

AGunBase::AGunBase()
{
	
}

void AGunBase::Stats_Initialize()
{
	MaxAmmo = 12;
	CurrentAmmo = MaxAmmo;
	RoundsPerSecond = 1.f;
	EffectiveRange = 1000.f;
	
	RelicBonus = 0;
	BaseDamage = 25.f;
	FinalDamage =RelicBonus + BaseDamage;
	TotalBonus = 1.0f;
	
	CanFire = true;
	ReloadTime = 1.2;
	ReloadingCheck= true;
	
	CritMultiplier = 2.0f;
}

void AGunBase::Fire_Gun(FVector Location, FVector Direction)
{
	// 발사
	CurrentAmmo -= 1;
	// 1. 끝점 계산
	FVector End = Location + (Direction * EffectiveRange);

	// 2. 파라미터 설정
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);           // 자기 자신 제외
	Params.AddIgnoredActor(GetOwner());    // 소유자(예: 캐릭터) 제외

	// 3. Line Trace 실행
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
		1.0f		// 두께
	);

	if (bHit)
	{
		BattleIn(HitResult);
	}
}

void AGunBase::BattleIn(const FHitResult& HitResult)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(HitResult.GetActor());
	UBattleSubsystem* BattleSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UBattleSubsystem>() : nullptr;

	if (!Monster || !BattleSubsystem) return;

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
	switch (parts)
	{
	case EPartsName::Bullet:
		if (Bullet.Level < 4)
		{
			Bullet.Value +=0.25;
			Bullet.Level ++;
		}
		break;
	case EPartsName::Scope:
		if (Scope.Level < 4)
		{
			Scope.Value +=0.20;
			Scope.Level ++;
		}
		break;
	case  EPartsName::Handle:
		if (Handle.Level < 4)
		{
			Handle.Value +=0.20;
			Handle.Level ++;
		}
		break;
	case  EPartsName::Magazine:
		if (Magazine.Level < 4)
		{
			Magazine.Value +=0.15;
			Magazine.Level ++;
		}
		break;
	default:
		break;
	}
}


void AGunBase::InitializeParts()
{
	Bullet.Name = "Bullet";
	Bullet.Level = 1;
	Bullet.Value = 1.0f;
	Bullet.Parts = EPartsName::Bullet;
	
	Magazine.Name = "Magazine";
	Magazine.Level = 1;
	Magazine.Value = 0;
	Magazine.Parts = EPartsName::Magazine;
	
	Scope.Name = "Scope";
	Scope.Level = 1;  
	Scope.Value = 0;  
	Scope.Parts = EPartsName::Scope;
	
	Handle.Name = "Handle";
	Handle.Level = 1;
	Handle.Value = 0;
	Handle.Parts = EPartsName::Handle;
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeParts();
	Stats_Initialize();
}
