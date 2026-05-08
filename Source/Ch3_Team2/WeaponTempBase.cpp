// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTempBase.h"

void UWeaponTempBase::Fire()
{
	if (!CanFire) return;
	if (CheckAmmo())
	{
		// 순서는 여기서 마음대로 

		PlayEffects();
		ProcessFiring();
		UpdateAmmo();

		Super::Fire();
		return;
	}
}

bool UWeaponTempBase::CheckAmmo_Implementation()
{
	
	return true;
}

void UWeaponTempBase::UpdateAmmo_Implementation()
{

}

void UWeaponTempBase::Reload_Implementation()
{
}
