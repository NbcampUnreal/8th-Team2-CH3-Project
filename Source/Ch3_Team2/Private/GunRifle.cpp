// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRifle.h"

void AGunRifle::FireGun(FVector Location, FVector Direction)
{
	// 발사	
	CurrentAmmo -= 1;
	Super::FireGun(Location, Direction);
}
