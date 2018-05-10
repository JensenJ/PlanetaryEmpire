// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"
#include "PECameraPawn.h"

APEPlayerController::APEPlayerController() {

}

void APEPlayerController::BeginPlay() {
	SetupPlayerInputComponent(InputComponent);
}
void APEPlayerController::Tick(float DeltaTime) {

}

///////////////////////////////////////////////////
///////////////// Player Input ////////////////////
///////////////////////////////////////////////////

void APEPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	EnableInput(this);
	check(InputComponent);
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Input Component not found"));
		return;
	}
	InputComponent->BindAxis("MoveForward", this, &APEPlayerController::InputMoveCameraForward);
	InputComponent->BindAxis("MoveRight", this, &APEPlayerController::InputMoveCameraRight);
	InputComponent->BindAxis("FastMove", this, &APEPlayerController::InputFastMoveCamera);
	InputComponent->BindAxis("Rotate", this, &APEPlayerController::InputRotateCamera);
	InputComponent->BindAxis("ZoomIn", this, &APEPlayerController::InputZoomInCamera);

}

void APEPlayerController::InputMoveCameraForward(float value) {
	if (!bCameraMoveable) return;
	CalculateMovementSpeed();
}
void APEPlayerController::InputMoveCameraRight(float value) {
	if (!bCameraMoveable) return;
}
void APEPlayerController::InputFastMoveCamera(float value) {
	if (!bCameraMoveable) return;
}
void APEPlayerController::InputRotateCamera(float value) {
	if (!bCameraMoveable) return;
}
void APEPlayerController::InputZoomInCamera(float value) {
	if (!bCameraMoveable) return;
}

///////////////////////////////////////////////////

void APEPlayerController::CalculateMovementSpeed() {
	
}