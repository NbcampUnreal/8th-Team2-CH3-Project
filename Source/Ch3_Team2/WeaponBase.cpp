// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);
	
	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GunMesh);

	AmmoPerFire = 1;
	CurrentAmmo = 0;
	MaxAmmo = 12;
	RoundsPerSecond = 1.f;
	CanFire = true;
	EffectiveRange = 1000.f;
	DamagePerHit = 100.f;
}

void AWeaponBase::Fire()
{
	CanFire = false;

	GetWorld()->GetTimerManager().SetTimer(TimerFireDelay, this, &AWeaponBase::HandleFireDelay, 1.f / RoundsPerSecond, false);
}

void AWeaponBase::HandleFireDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerFireDelay);

	CanFire = true;
}

