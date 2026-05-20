// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPistol.h"

void AGunPistol::Fire_Gun(FVector Location, FVector Direction)
{
	// 발사	
	CurrentAmmo -= 1;
	Super::Fire_Gun(Location, Direction);
}
