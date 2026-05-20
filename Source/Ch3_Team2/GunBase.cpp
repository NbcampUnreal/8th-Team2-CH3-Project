// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "Battle/BattleSubsystem.h"
#include "public/MonsterBase.h"
#include "DrawDebugHelpers.h"

bool AGunBase::CheckAmmo()
{
	if (CanFire && ReloadingCheck == false && CurrentAmmo > 0)
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
AGunBase::AGunBase()
{
	
}
bool AGunBase::CheckReload()
{
	// 총알이 가득 차있을 떄 
	if (CurrentAmmo < MaxAmmo && ReloadingCheck == false )
	{
		ReloadingCheck = true;
		return true;
	}
	return false;
}

void AGunBase::Reloading()
{
	// 재장전 되었을 떄 
	ReloadingCheck = false;
	CurrentAmmo = MaxAmmo;
	UE_LOG(LogTemp, Log, TEXT("Reloading!!!!"));
	
}

void AGunBase::Fire_Gun(FVector Location, FVector Direction)
{
	// 1. 끝점 계산
	//FVector End = Location + (Direction * EffectiveRange);
	// 여기서 원래 똑바르던 Direction이 무작위로 튄 SpreadDirection으로 재탄생합니다.
	FVector SpreadDirection = FMath::VRandCone(Direction, FMath::DegreesToRadians(SpreadAngleDegrees ));

	// [단계 2] 최종 도착점(End) 계산
	// 원래 Direction 대신, 탄퍼짐이 적용된 SpreadDirection을 사거리에 곱해 최종 목적지를 구합니다.
	FVector End = Location + (SpreadDirection * EffectiveRange);

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

	if (!Monster || !BattleSubsystem) return;

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
	switch (parts)
	{
	case EPartsName::Bullet:
		if (Bullet.Level < MaxLevelParts)
		{
			Bullet.Value += LevelUpDamageValue;
			++Bullet.Level;
			// 탄환 데미지가 올랐으므로 최종 데미지 공식 실시간 재계산
			AddDamage(0.0f, 0.0f, 0.0f); 
		}
		break;
	case EPartsName::Scope:
		if (Scope.Level < MaxLevelParts)
		{
			Scope.Value += LevelUpScopeValue;
			++Scope.Level;
			// 필요 시 크리티컬 확률이나 조준 보정 로직을 이곳에 연동
		}
		break;
	case EPartsName::Handle:
		if (Handle.Level < MaxLevelParts)
		{
			Handle.Value += LevelUpHandleValue;
			++Handle.Level;
			// 반동 감소 로직이나 재장전 속도 변경이 필요하다면 여기서 연동
		}
		break;
	case EPartsName::Magazine:
		if (Magazine.Level < MaxLevelParts)
		{
			// 1탄창 강화 시 탄수 수치 증가
			Magazine.Value += LevelUpReloadValue;
			++Magazine.Level;
            
			// 기획에 맞춘 실질적인 최대 탄수 증가 연동 (예: 레벨당 5발 추가)
			AddAmmo(5.0f); 
		}
		break;
	default:
		break;
	}
}

FGunParts AGunBase::GetPartsData(EPartsName PartsType) const
{
	switch (PartsType)
	{
	case EPartsName::Bullet:   
		return Bullet;
	case EPartsName::Magazine:
		return Magazine;
	case EPartsName::Scope:    
		return Scope;
	case EPartsName::Handle:   
		return Handle;
	default:                   
		return FGunParts();
	}
}

void AGunBase::AddReloadStat(float AddReload)
{
	if (ReloadTime + AddReload >= 0.1f)
	{
		ReloadTime += AddReload;
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
}
