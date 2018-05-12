// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"
#include "PECameraPawn.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

APEPlayerController::APEPlayerController() {
	
}

APECameraPawn* APEPlayerController::GetControlledPawn() const{
	return Cast<APECameraPawn>(GetPawn());
}

void APEPlayerController::BeginPlay() {
	SetupPlayerInputComponent(InputComponent);

	// Get camera pawn class
	CameraPawn = GetControlledPawn();
	if (!CameraPawn) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: CameraPawn Class not found"));
		return;
	}
	LocalSceneComponent = CameraPawn->GetSceneComponent();
	LocalSphereComponent = CameraPawn->GetSphereComponent();
	LocalSpringArmComponent = CameraPawn->GetSpringArmComponent();
	LocalCameraComponent = CameraPawn->GetCameraComponent();
}
void APEPlayerController::Tick(float DeltaTime) {

}

///////////////////////////////////////////////////
///////////////// Player Input ////////////////////
///////////////////////////////////////////////////

void APEPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	EnableInput(this);
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Input Component not found::PointerProtection"));
		return;
	}
	InputComponent->BindAxis("MoveForward", this, &APEPlayerController::InputMoveCameraForward);
	InputComponent->BindAxis("MoveRight", this, &APEPlayerController::InputMoveCameraRight);
	InputComponent->BindAxis("FastMove", this, &APEPlayerController::InputFastMoveCamera);
	InputComponent->BindAxis("Rotate", this, &APEPlayerController::InputRotateCamera);
	InputComponent->BindAxis("ZoomIn", this, &APEPlayerController::InputZoomInCamera);

}

void APEPlayerController::InputMoveCameraForward(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
	MovementSpeed = CalculateMovementSpeed();
	FTransform FinalTransform = MovementX(MovementSpeed, FastMoveMultiplier, AxisValue);
	CameraPawn->SetActorTransform(FinalTransform);
}

void APEPlayerController::InputMoveCameraRight(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
	MovementSpeed = CalculateMovementSpeed();
	FTransform FinalTransform = MovementY(MovementSpeed, FastMoveMultiplier, AxisValue);
	CameraPawn->SetActorTransform(FinalTransform);
}

void APEPlayerController::InputFastMoveCamera(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
}

void APEPlayerController::InputRotateCamera(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
}

void APEPlayerController::InputZoomInCamera(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
}


///////////////////////////////////////////////////
/////////////// Camera Calculations ///////////////
///////////////////////////////////////////////////

float APEPlayerController::CalculateMovementSpeed() {
	if (!LocalSpringArmComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Calculation of movement speed X failed::LocalSpringArmComponent"));
		return 100.0f;
	}
	float LocalMovementSpeed = LocalSpringArmComponent->TargetArmLength / 100;
	LocalMovementSpeed = FMath::Clamp(LocalMovementSpeed, 5.0f, 20.0f);
	if (!LocalMovementSpeed) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Calculation of movement speed X failed::LocalMovementSpeed"));
		return 100.0f;
	}
	//UE_LOG(LogTemp, Warning, TEXT("PEPlayerController: Movement speed: %f"), LocalMovementSpeed);
	return LocalMovementSpeed;
}

FTransform APEPlayerController::MovementX(float AxisValue, float MovementSpeed, float SpeedMultiplier) {
	MovementSpeed = (MovementSpeed * SpeedMultiplier) * AxisValue;

	FVector ForwardMovementVector = FVector(MovementSpeed, 0.0f, 0.0f);
	FTransform CameraPawnTransform = CameraPawn->GetActorTransform();
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(CameraPawnTransform, ForwardMovementVector);

	FVector OutTransform;
	FRotator OutRotator;
	FVector OutScale;
	UKismetMathLibrary::BreakTransform(CameraPawnTransform, OutTransform, OutRotator, OutScale);
	FVector AddedVector = TransformDirection + OutTransform;
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(AddedVector, OutRotator, OutScale);
	return FinalTransform;
}

FTransform APEPlayerController::MovementY(float AxisValue, float MovementSpeed, float SpeedMultiplier) {
	MovementSpeed = (MovementSpeed * SpeedMultiplier) * AxisValue;

	FVector ForwardMovementVector = FVector(0.0f, MovementSpeed, 0.0f);
	FTransform CameraPawnTransform = CameraPawn->GetActorTransform();
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(CameraPawnTransform, ForwardMovementVector);

	FVector OutTransform;
	FRotator OutRotator;
	FVector OutScale;
	UKismetMathLibrary::BreakTransform(CameraPawnTransform, OutTransform, OutRotator, OutScale);
	FVector AddedVector = TransformDirection + OutTransform;
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(AddedVector, OutRotator, OutScale);
	return FinalTransform;
}

///////////////////////////////////////////////////