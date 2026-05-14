// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LobbyWidget.generated.h"

class UButton;

UCLASS()
class CH3_TEAM2_API ULobbyWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_PlayGame;

	UFUNCTION()
	void OnPlayButtonClicked();
};
