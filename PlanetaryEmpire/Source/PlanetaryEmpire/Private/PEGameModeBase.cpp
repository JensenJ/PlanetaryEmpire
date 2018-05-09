// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameModeBase.h"
#include "PEPlayerController.h"
#include "PEPlayerState.h"
#include "PEGameStateBase.h"

APEGameModeBase::APEGameModeBase() {
	PlayerControllerClass = APEPlayerController::StaticClass();
	PlayerStateClass = APEPlayerState::StaticClass();
	GameStateClass = APEGameStateBase::StaticClass();
}