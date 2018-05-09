// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"
#include "PECameraPawn.h"

APEPlayerController::APEPlayerController() {
	CameraPawnRef = GetCameraPawnReference();
}

void APEPlayerController::BeginPlay() {
	if (!CameraPawnRef) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Camera Pawn Reference not found"));
		return;
	}
	SetupPlayerInputComponent(InputComponent);

}
void APEPlayerController::Tick(float DeltaTime) {

}
APawn* APEPlayerController::GetCameraPawnReference() const{
	return Cast<APECameraPawn>(GetPawn());
}

void APEPlayerController::SetupPlayerInputComponent(UInputComponent* InputComponent){
	EnableInput(this);
	check(InputComponent);
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Input Component not found"));
		return;
	}
	InputComponent->BindAxis(FName("MoveForward"), this, &APEPlayerController::InputMoveCameraForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &APEPlayerController::InputMoveCameraRight);
	InputComponent->BindAxis(FName("FastMove"), this, &APEPlayerController::InputFastMoveCamera);
	InputComponent->BindAxis(FName("Rotate"), this, &APEPlayerController::InputRotateCamera);
	InputComponent->BindAxis(FName("ZoomIn"), this, &APEPlayerController::InputZoomInCamera);

}

void APEPlayerController::InputMoveCameraForward() {

}
void APEPlayerController::InputMoveCameraRight() {

}
void APEPlayerController::InputFastMoveCamera() {

}
void APEPlayerController::InputRotateCamera() {

}
void APEPlayerController::InputZoomInCamera() {

}