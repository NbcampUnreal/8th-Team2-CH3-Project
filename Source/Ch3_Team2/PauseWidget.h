// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PauseWidget.generated.h"

class UButton;

UCLASS()
class CH3_TEAM2_API UPauseWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UPauseWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// 위젯이 화면에 켜질 때 (스택에 추가될 때)
	virtual void NativeOnActivated() override;

	// 위젯이 화면에서 꺼질 때 (스택에서 제거될 때)
	virtual void NativeOnDeactivated() override;

	// 입력 모드 설정 (UI 포커스)
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	
	void NativeConstruct();

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoToMainMenu;
	
	UFUNCTION()
	void OnMainMenuButtonClicked();
};
