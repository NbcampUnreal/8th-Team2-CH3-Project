#include "GunPistol.h"
void AGunPistol::Fire_Gun(FVector Location, FVector Direction)
{
	// 발사	
	CurrentAmmo -= 1;
	Super::Fire_Gun(Location, Direction);
}
