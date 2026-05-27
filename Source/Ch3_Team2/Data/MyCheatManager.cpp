#include "Data/MyCheatManager.h"

#include "APlayer.h"
#include "LevelFlowSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UMyCheatManager::Next()
{
	APlayerController* PC = GetOuterAPlayerController();
	if (!PC) return;

	UGameInstance* GI = PC->GetGameInstance();
	if (!GI) return;

	ULevelFlowSubsystem* LevelFlow = GI->GetSubsystem<ULevelFlowSubsystem>();
	if (!LevelFlow) return;
	
	LevelFlow->TravelToNextLevel();
}

void UMyCheatManager::Relic()
{
	APlayerController* PC = GetOuterAPlayerController();
	if (!PC) return;

	PC->ProcessEvent(
		PC->FindFunction(FName("CallRelicCheat")),
		nullptr
	);
}

void UMyCheatManager::LevelUp()
{
	AAPlayer* Player = Cast<AAPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->AddExp(100000);
}
