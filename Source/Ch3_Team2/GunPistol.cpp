#include "GunPistol.h"

void AGunPistol::FireGun(FVector Location, FVector Direction)
{
	
	Super::FireGun(Location, Direction);
	--CurrentAmmo;
}