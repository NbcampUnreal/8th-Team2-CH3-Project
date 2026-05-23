// Fill out your copyright notice in the Description page of Project Settings.


#include "SBW/GunShotGun.h"

void AGunShotGun::FireGun(FVector Location, FVector Direction)
{
	// 발사	
	CurrentAmmo -= 1;
	
	for (int32 i = 0; i < MultiAmmo; i++)
	{
		Super::FireGun(Location, Direction);
	}
}
