// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRifle.h"

void AGunRifle::FireGun(FVector Location, FVector Direction)
{
	Super::FireGun(Location, Direction);
	--CurrentAmmo;
}
