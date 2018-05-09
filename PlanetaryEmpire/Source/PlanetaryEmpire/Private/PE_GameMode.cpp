// Fill out your copyright notice in the Description page of Project Settings.

#include "PE_GameMode.h"
#include "PE_PlayerController.h"
#include "PE_CameraSpecPawn.h"

APE_GameMode::APE_GameMode() {
	PlayerControllerClass = APE_PlayerController::StaticClass();
	DefaultPawnClass = APE_CameraSpecPawn::StaticClass();
}