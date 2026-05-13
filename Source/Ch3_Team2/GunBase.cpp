// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

bool AGunBase::CheckAmmo_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Checking : %s "),CurrentAmmo <= 0 ? TEXT("true")
		: TEXT("False"));
	return CurrentAmmo <= 0;
}

void AGunBase::UpdateAmmo_Implementation()
{
	CurrentAmmo -= 1;
}

void AGunBase::Reload_Implementation()
{
	CurrentAmmo = MaxAmmo;
}

AGunBase::AGunBase()
{
	
}

void AGunBase::Fire()
{
	if (!CanFire) return;
	if (CheckAmmo())
	{
		// 순서는 여기서 마음대로 
		//PlayEffects();
		//ProcessFiring();
		UpdateAmmo();
	
		Super::Fire();
		return;
	}
}
