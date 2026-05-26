#include "AGameMode.h"
#include "APlayer.h"
#include "AGameState.h"
#include "GunBase.h"
#include "RelicManager.h"
#include "Data/SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

AAGameMode::AAGameMode()
{
	DefaultPawnClass = AAPlayer::StaticClass();
	GameStateClass = AAGameState::StaticClass();
	
}

void AAGameMode::BeginPlay()
{
	Super::BeginPlay();
	USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	if (!IsValid(SaveSubsystem))
	{
		return;
	}
	ARelicManager* Relic = Cast<ARelicManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARelicManager::StaticClass()));
	if (!IsValid(Relic))
	{
		return;
	}
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AAPlayer::StaticClass()));
	if (!IsValid(Player))
	{
		return;
	}
	AGunBase* Gun = Cast<AGunBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGunBase::StaticClass()));
	if (!IsValid(Gun))
	{
		return;
	}
	Relic->LoadData(SaveSubsystem->GetRelicIDs());
	Player->LoadData(SaveSubsystem->GetPlayerLevel(), SaveSubsystem->GetPlayerSkill(), SaveSubsystem->GetPlayerWeapon());
	Gun->LoadData(SaveSubsystem->GetGripLevel(), SaveSubsystem->GetScopeLevel(), SaveSubsystem->GetMagazineLevel(), SaveSubsystem->GetBulletLevel());
}
