#include "GunPistol.h"
void AGunPistol::FireGun(FVector Location, FVector Direction)
{
	// 발사	
	CurrentAmmo -= 1;
	Super::FireGun(Location, Direction);
}
