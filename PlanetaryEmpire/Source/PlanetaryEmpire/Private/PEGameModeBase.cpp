// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameModeBase.h"
#include "PEPlayerController.h"
#include "PEPlayerState.h"
#include "PEGameStateBase.h"
#include "PECameraPawn.h"

APEGameModeBase::APEGameModeBase() {
	PlayerControllerClass = APEPlayerController::StaticClass();
	DefaultPawnClass = APECameraPawn::StaticClass();
	PlayerStateClass = APEPlayerState::StaticClass();
	GameStateClass = APEGameStateBase::StaticClass();
}