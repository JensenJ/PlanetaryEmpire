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

///////////////////////////////////////////////////
///////////////// Player Input ////////////////////
///////////////////////////////////////////////////

void APEPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	EnableInput(this);
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Input Component not found::PointerProtection"));
		return;
	}
	InputComponent->BindAction("TogglePan", IE_Pressed, this, &APEPlayerController::InputTogglePan);
	InputComponent->BindAction("FastMove", IE_Pressed, this, &APEPlayerController::InputToggleFastMove);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APEPlayerController::InputZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &APEPlayerController::InputZoomOut);

	InputComponent->BindAxis("MoveForward", this, &APEPlayerController::InputMoveCameraForward);
	InputComponent->BindAxis("MoveRight", this, &APEPlayerController::InputMoveCameraRight);
	InputComponent->BindAxis("XMovement", this, &APEPlayerController::InputCameraX);
	InputComponent->BindAxis("YMovement", this, &APEPlayerController::InputCameraY);

}

void APEPlayerController::InputMoveCameraForward(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
	MovementSpeed = CalculateMovementSpeed();
	CameraPawn->SetActorTransform(MovementX(MovementSpeed, FastMoveMultiplier, AxisValue));
}

void APEPlayerController::InputMoveCameraRight(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
	MovementSpeed = CalculateMovementSpeed();
	CameraPawn->SetActorTransform(MovementY(AxisValue, MovementSpeed, FastMoveMultiplier));
}

void APEPlayerController::InputCameraX(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;

	if (bPanToggled == true) {
		CameraPawn->SetActorRotation(PanX(AxisValue, PanSensitivity));
	}
	float XDeltaSpeed, YDeltaSpeed;
	ScreenEdgeMovement(AxisValue, XDeltaSpeed, YDeltaSpeed);
	FVector LocalOffsetVector = UKismetMathLibrary::MakeVector(XDeltaSpeed, YDeltaSpeed, 0.0f);
	CameraPawn->AddActorLocalOffset(LocalOffsetVector, true);
}

void APEPlayerController::InputCameraY(float AxisValue) {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;

	if (bPanToggled == true) {
		CameraPawn->SetActorRotation(PanY(AxisValue, PanSensitivity));
	}
	float XDeltaSpeed, YDeltaSpeed;
	ScreenEdgeMovement(AxisValue, XDeltaSpeed, YDeltaSpeed);
	FVector LocalOffsetVector = UKismetMathLibrary::MakeVector(XDeltaSpeed, YDeltaSpeed, 0.0f);
	CameraPawn->AddActorLocalOffset(LocalOffsetVector, true);
}


void APEPlayerController::InputTogglePan() {
	if (!bCameraMoveable) return;
	bPanToggled = !bPanToggled;
}

void APEPlayerController::InputToggleFastMove() {
	if (!bCameraMoveable) return;
	bFastMove = !bFastMove;
	if (bFastMove) {
		FastMoveMultiplier = 2.0f;
	}
	else {
		FastMoveMultiplier = 1.0f;
	}
}
void APEPlayerController::InputZoomIn() {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
	float ZoomValue = LocalSpringArmComponent->TargetArmLength - ZoomSensitivity;
	ZoomValue = FMath::Clamp(ZoomValue, MinArmDistance, MaxArmDistance);
	LocalSpringArmComponent->TargetArmLength = ZoomValue;
}

void APEPlayerController::InputZoomOut() {
	if (!CameraPawn) return;
	if (!bCameraMoveable) return;
	float ZoomValue = LocalSpringArmComponent->TargetArmLength + ZoomSensitivity;
	ZoomValue = FMath::Clamp(ZoomValue, MinArmDistance, MaxArmDistance);
	LocalSpringArmComponent->TargetArmLength = ZoomValue;
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
	float x, y, z;
	UKismetMathLibrary::BreakVector(AddedVector, x, y, z);
	z = 100;
	FVector FinalVector = UKismetMathLibrary::MakeVector(x, y, z);
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(FinalVector, OutRotator, OutScale);
	return FinalTransform;
}

FRotator APEPlayerController::PanX(float AxisValue, float PanSensitivity) {
	PanSensitivity = PanSensitivity * AxisValue;
	FRotator CameraPawnRot = CameraPawn->GetActorRotation();
	float RotatorRoll, RotatorPitch, RotatorYaw;
	UKismetMathLibrary::BreakRotator(CameraPawnRot, RotatorRoll, RotatorPitch, RotatorYaw);
	PanSensitivity = PanSensitivity + RotatorYaw;
	FRotator FinalRotator = UKismetMathLibrary::MakeRotator(RotatorRoll, RotatorPitch, PanSensitivity);
	return FinalRotator;
}

FRotator APEPlayerController::PanY(float AxisValue, float PanSensitivity) {
	PanSensitivity = PanSensitivity * AxisValue;
	FRotator CameraPawnRot = CameraPawn->GetActorRotation();
	float RotatorRoll, RotatorPitch, RotatorYaw;
	UKismetMathLibrary::BreakRotator(CameraPawnRot, RotatorRoll, RotatorPitch, RotatorYaw);
	PanSensitivity = PanSensitivity + RotatorPitch;
	PanSensitivity = FMath::Clamp(PanSensitivity, -80.0f, -15.0f);
	FRotator FinalRotator = UKismetMathLibrary::MakeRotator(RotatorRoll, PanSensitivity, RotatorYaw);
	return FinalRotator;
}

void APEPlayerController::ScreenEdgeMovement(float AxisValue, float &DeltaXSpeed, float &DeltaYSpeed) {
	int SizeX, SizeY;
	GetViewportSize(SizeX, SizeY);
	float XLocation, YLocation;
	GetMousePosition(XLocation, YLocation);
	float CursorLocationX = XLocation / SizeX;
	float CursorLocationY = YLocation / SizeY;
	if (CursorLocationX >= 0.975f) {
		EdgeMoveSpeedX = 5.0f;
	}
	else {
		if (CursorLocationX <= 0.025f) {
			EdgeMoveSpeedX = -5.0f;
		}
		else {
			EdgeMoveSpeedX = 0.0f;
		}
	}
	if (CursorLocationY >= 0.975f) {
		EdgeMoveSpeedY = -5.0f;
	}
	else {
		if (CursorLocationY <= 0.025f) {
			EdgeMoveSpeedY = 5.0f;
		}
		else {
			EdgeMoveSpeedY = 0.0f;
		}
	}
	DeltaXSpeed = EdgeMoveSpeedY * FastMoveMultiplier;
	DeltaYSpeed = EdgeMoveSpeedX * FastMoveMultiplier;
}