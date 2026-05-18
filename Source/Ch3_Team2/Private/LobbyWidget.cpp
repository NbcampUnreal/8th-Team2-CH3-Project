#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Data/LevelFlowSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_PlayGame)
	{
		Btn_PlayGame->OnClicked.AddDynamic(this, &ULobbyWidget::OnPlayButtonClicked);
	}
}

void ULobbyWidget::OnPlayButtonClicked()
{
	if (GEngine)
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
	{
		if (ULevelFlowSubsystem* FlowSystem = GI->GetSubsystem<ULevelFlowSubsystem>())
		{
			FlowSystem->TravelToNextLevel(); // 게임 레벨로 이동
		}
	}
}

TOptional<FUIInputConfig> ULobbyWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}