// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameUIRoot.generated.h"

class UCommonActivatableWidgetStack;

UCLASS()
class CH3_TEAM2_API UGameUIRoot : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	// 인게임 HUD (전투, 생존 정보)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameLayerStack;

	// 메뉴 (메인 로비, 상점, 설정)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuLayerStack;

	// 팝업 (일시정지, 보상 창)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ModalLayerStack;
};
