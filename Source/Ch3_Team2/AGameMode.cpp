// Fill out your copyright notice in the Description page of Project Settings.


#include "AGameMode.h"
#include "APlayer.h"
#include "AGameState.h"

AAGameMode::AAGameMode()
{
	// 클레스 이름을 통해서 클레스를 반환해주는 시스템 이다.
	// PawnClass - 케릭터 Class
	DefaultPawnClass = AAPlayer::StaticClass();
	// 플레이어 Controller
	//PlayerControllerClass = AProjectAPlayerController::StaticClass();
	// Game State 설정
	GameStateClass = AAGameState::StaticClass();
}
