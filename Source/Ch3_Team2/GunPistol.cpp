#include "GunPistol.h"

void AGunPistol::FireGun(FVector Location, FVector Direction)
{
	UE_LOG(LogTemp, Error, TEXT("🎯 [GunPistol] 자식 권총의 FireGun 내부 진입 완벽 성공!"));
	Super::FireGun(Location, Direction);
	--CurrentAmmo;
}