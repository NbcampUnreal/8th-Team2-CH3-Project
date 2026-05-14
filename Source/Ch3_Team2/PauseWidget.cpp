#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Data/LevelFlowSubsystem.h"
#include "Components/Button.h"

UPauseWidget::UPauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ESC를 누르면 창이 자동으로 닫히도록 설정
	bIsBackHandler = true; 
}

void UPauseWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
    
	// 창이 켜지면 게임을 일시정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UPauseWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
    
	// 창이 꺼지면 게임 일시정지를 해제
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		// 마우스 커서를 보이지 않게 설정
		PC->bShowMouseCursor = false;
        
		// 입력을 다시 게임(캐릭터 조작)으로만 전달하도록 설정
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

TOptional<FUIInputConfig> UPauseWidget::GetDesiredInputConfig() const
{
	// 캐릭터 조작을 막고 마우스/키보드 입력을 UI에만 집중
	return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);
}

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_GoToMainMenu)
	{
		Btn_GoToMainMenu->OnClicked.AddDynamic(this, &UPauseWidget::OnMainMenuButtonClicked);
	}
}

void UPauseWidget::OnMainMenuButtonClicked()
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
	{
		if (ULevelFlowSubsystem* FlowSystem = GI->GetSubsystem<ULevelFlowSubsystem>())
		{
			FlowSystem->TravelToLevelByIndex(0); 
		}
	}
}