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
	ARelicManager* Relic = Cast<ARelicManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARelicManager::StaticClass()));
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AAPlayer::StaticClass()));
	AGunBase* Gun = Cast<AGunBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGunBase::StaticClass()));
	
	// TODO: 순서 보장하면서 값만 넘기기
	// Relic->LoadData(SaveSubsystem->RelicIds);
	//Player->LoadData(SaveSubsystem->GetPlayerLevel());
	// Gun->LoadData();
}
