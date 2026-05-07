// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

bool AGunBase::CheckAmmo_Implementation()
{
	return AmmoPerFire <= CurrentAmmo;
}

void AGunBase::UpdateAmmo_Implementation()
{
	CurrentAmmo -= AmmoPerFire;
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
