// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"
#include "PECameraPawn.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

APEPlayerController::APEPlayerController() {

}

// Retrieves the PECameraPawn class
APECameraPawn* APEPlayerController::GetControlledPawn() const{
	return Cast<APECameraPawn>(GetPawn());
}

void APEPlayerController::BeginPlay() {
	SetupPlayerInputComponent(InputComponent);

	// Get camera pawn class
	CameraPawn = GetControlledPawn();
	// Check if it was found
	if (!CameraPawn) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: CameraPawn Class not found"));
		return;
	}
	//Declare local components for easy access
	LocalSceneComponent = CameraPawn->GetSceneComponent();
	LocalSphereComponent = CameraPawn->GetSphereComponent();
	LocalSpringArmComponent = CameraPawn->GetSpringArmComponent();
	LocalCameraComponent = CameraPawn->GetCameraComponent();
}

///////////////////////////////////////////////////
///////////////// Player Input ////////////////////
///////////////////////////////////////////////////

void APEPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	//Check for input component
	EnableInput(this);
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Input Component not found::PointerProtection"));
		return;
	}
	// Binding actions
	InputComponent->BindAction("TogglePan", IE_Pressed, this, &APEPlayerController::InputTogglePan);
	InputComponent->BindAction("FastMove", IE_Pressed, this, &APEPlayerController::InputToggleFastMove);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APEPlayerController::InputZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &APEPlayerController::InputZoomOut);
	// Binding axis
	InputComponent->BindAxis("MoveForward", this, &APEPlayerController::InputMoveCameraForward);
	InputComponent->BindAxis("MoveRight", this, &APEPlayerController::InputMoveCameraRight);
	InputComponent->BindAxis("XMovement", this, &APEPlayerController::InputCameraX);
	InputComponent->BindAxis("YMovement", this, &APEPlayerController::InputCameraY);
}

// Sets pawn's transform on X axis based on calculated movement speed
void APEPlayerController::InputMoveCameraForward(float AxisValue) {
	if (!CameraPawn) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	MovementSpeed = CalculateMovementSpeed();
	CameraPawn->SetActorTransform(MovementX(MovementSpeed, FastMoveMultiplier, AxisValue));
}

// Sets pawn's transform on Y axis based on calculated movement speed
void APEPlayerController::InputMoveCameraRight(float AxisValue) {
	if (!CameraPawn) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	MovementSpeed = CalculateMovementSpeed();
	CameraPawn->SetActorTransform(MovementY(AxisValue, MovementSpeed, FastMoveMultiplier));
}

// Gets input from X movement on mouse or game pad
void APEPlayerController::InputCameraX(float AxisValue) {
	if (!CameraPawn) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not

	if (bPanToggled == true) { //Sets actor rotation if camera is in panning mode
		CameraPawn->SetActorRotation(PanX(AxisValue, PanSensitivity));
	}
	float XDeltaSpeed, YDeltaSpeed; // Declaring OUT parameters
	ScreenEdgeMovement(AxisValue, XDeltaSpeed, YDeltaSpeed); // Calling ScreenEdgeMovement method
	FVector LocalOffsetVector = UKismetMathLibrary::MakeVector(XDeltaSpeed, YDeltaSpeed, 0.0f); //Making vector based on this
	CameraPawn->AddActorLocalOffset(LocalOffsetVector, true); // Adding offset from vector to camera pawn
}

// Gets input from Y movement on mouse or game pad
void APEPlayerController::InputCameraY(float AxisValue) {
	if (!CameraPawn) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not

	if (bPanToggled == true) { //Sets actor rotation if camera is in panning mode
		CameraPawn->SetActorRotation(PanY(AxisValue, PanSensitivity));
	}
	float XDeltaSpeed, YDeltaSpeed; // Declaring OUT parameters
	ScreenEdgeMovement(AxisValue, XDeltaSpeed, YDeltaSpeed); // Calling ScreenEdgeMovement method
	FVector LocalOffsetVector = UKismetMathLibrary::MakeVector(XDeltaSpeed, YDeltaSpeed, 0.0f); //Making vector based on this
	CameraPawn->AddActorLocalOffset(LocalOffsetVector, true); // Adding offset from vector to camera pawn
}

// Toggles panning
void APEPlayerController::InputTogglePan() {
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	bPanToggled = !bPanToggled; // Toggles whether camera is panning or not
}

// Toggles fast moving
void APEPlayerController::InputToggleFastMove() {
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	bFastMove = !bFastMove; // Toggles whether camera is fast moving or not
	if (bFastMove) { //Checks for fast move and increases speed if true
		FastMoveMultiplier = 2.0f;
	}
	else { // Otherwise it is kept at normal speed
		FastMoveMultiplier = 1.0f;
	}
}

// Sets new zoom in value
void APEPlayerController::InputZoomIn() {
	if (!CameraPawn) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	float ZoomValue = LocalSpringArmComponent->TargetArmLength - ZoomSensitivity; //Takes zoom sensitivity away from target arm length
	ZoomValue = FMath::Clamp(ZoomValue, MinArmDistance, MaxArmDistance); // Clamps values between min and max
	LocalSpringArmComponent->TargetArmLength = ZoomValue; // Sets new spring arm length
}
// Sets new zoom out value
void APEPlayerController::InputZoomOut() {
	if(!CameraPawn) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	float ZoomValue = LocalSpringArmComponent->TargetArmLength + ZoomSensitivity; // Adds zoom sensitivity to target arm length
	ZoomValue = FMath::Clamp(ZoomValue, MinArmDistance, MaxArmDistance); // Clamps values between min and max
	LocalSpringArmComponent->TargetArmLength = ZoomValue; // Sets new spring arm length
}
///////////////////////////////////////////////////
/////////////// Camera Calculations ///////////////
///////////////////////////////////////////////////

// Calculates movement speed
float APEPlayerController::CalculateMovementSpeed() {
	if (!LocalSpringArmComponent) { //Checks for spring arm component
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Calculation of movement speed X failed::LocalSpringArmComponent"));
		return 100.0f; // returns 100 as a default
	}
	float LocalMovementSpeed = LocalSpringArmComponent->TargetArmLength / 100; // divides target arm length by 100 to get value to clamp
	LocalMovementSpeed = FMath::Clamp(LocalMovementSpeed, MovementSpeedMin, MovementSpeedMax); // clamp value between min and max
	return LocalMovementSpeed; // Returns local movement speed
}

// Calculates movement on X axis
FTransform APEPlayerController::MovementX(float AxisValue, float MovementSpeed, float SpeedMultiplier) {
	MovementSpeed = (MovementSpeed * SpeedMultiplier) * AxisValue; // Works out base movement speed

	FVector ForwardMovementVector = FVector(MovementSpeed, 0.0f, 0.0f); // Constructs a new vector based on new value
	FTransform CameraPawnTransform = CameraPawn->GetActorTransform(); // Gets camera pawn transform
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(CameraPawnTransform, ForwardMovementVector); // Creates a transform direction from vectors and transform
	// Declaring OUT variables
	FVector OutTransform;
	FRotator OutRotator;
	FVector OutScale;
	UKismetMathLibrary::BreakTransform(CameraPawnTransform, OutTransform, OutRotator, OutScale); //Breaks transform
	FVector AddedVector = TransformDirection + OutTransform; // Adds transforms together
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(AddedVector, OutRotator, OutScale); // Creates a new transform with inputted values
	return FinalTransform; // Returns final transform
}

// Calculates movement on Y axis
FTransform APEPlayerController::MovementY(float AxisValue, float MovementSpeed, float SpeedMultiplier) {
	MovementSpeed = (MovementSpeed * SpeedMultiplier) * AxisValue; // Works out base movement speed

	FVector ForwardMovementVector = FVector(0.0f, MovementSpeed, 0.0f); // Constructs a new vector based on new values
	FTransform CameraPawnTransform = CameraPawn->GetActorTransform(); // Gets camera pawn transform
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(CameraPawnTransform, ForwardMovementVector); // Creates a transform direction from vectors and transform
	// Declaring OUT variables
	FVector OutTransform;
	FRotator OutRotator;
	FVector OutScale;
	UKismetMathLibrary::BreakTransform(CameraPawnTransform, OutTransform, OutRotator, OutScale); //Breaks transform
	FVector AddedVector = TransformDirection + OutTransform; // Adds transforms together
	float x, y, z; // Declaring more OUT variables
	UKismetMathLibrary::BreakVector(AddedVector, x, y, z);
	z = 100; // Forces Z axis to be 100, preventing bug of changing height constantly
	FVector FinalVector = UKismetMathLibrary::MakeVector(x, y, z); // Constructs a new vector based on new values
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(FinalVector, OutRotator, OutScale); // Creates a new transform with inputted values
	return FinalTransform; // Returns final transform
}

// Calculations for panning on the X axis
FRotator APEPlayerController::PanX(float AxisValue, float PanSensitivity) {
	PanSensitivity = PanSensitivity * AxisValue; // Works out pan sensitivity
	FRotator CameraPawnRot = CameraPawn->GetActorRotation(); // Gets camera pawn rotation
	float RotatorRoll, RotatorPitch, RotatorYaw; // Declaring OUT variables
	UKismetMathLibrary::BreakRotator(CameraPawnRot, RotatorRoll, RotatorPitch, RotatorYaw); // Breaking rotator
	PanSensitivity = PanSensitivity + RotatorYaw; // Adds yaw to pan sensitivity
	FRotator FinalRotator = UKismetMathLibrary::MakeRotator(RotatorRoll, RotatorPitch, PanSensitivity); // Constructs final rotator based on new values
	return FinalRotator; // Returns final rotator
}

// Calculations for panning on the Y axis
FRotator APEPlayerController::PanY(float AxisValue, float PanSensitivity) {
	PanSensitivity = PanSensitivity * AxisValue; // Works out pan sensitivity
	FRotator CameraPawnRot = CameraPawn->GetActorRotation(); // Gets camera pawn rotation
	float RotatorRoll, RotatorPitch, RotatorYaw; // Declaring OUT variables
	UKismetMathLibrary::BreakRotator(CameraPawnRot, RotatorRoll, RotatorPitch, RotatorYaw); // Breaking rotator
	PanSensitivity = PanSensitivity + RotatorPitch; // Adds yaw to pan sensitivity
	PanSensitivity = FMath::Clamp(PanSensitivity, -80.0f, -15.0f); // Clamps values to avoid rotating under the map or above the pivot point, which would cause inverted controls
	FRotator FinalRotator = UKismetMathLibrary::MakeRotator(RotatorRoll, PanSensitivity, RotatorYaw); // Constructs final rotator based on new values
	return FinalRotator; // Returns final rotator
}

// Calculates when to use screen edge movements
void APEPlayerController::ScreenEdgeMovement(float AxisValue, float &DeltaXSpeed, float &DeltaYSpeed) {
	int SizeX, SizeY; // Declaring OUT variables
	GetViewportSize(SizeX, SizeY); // Gets the viewport size
	float XLocation, YLocation; // Declaring more OUT variables
	GetMousePosition(XLocation, YLocation); // Gets the mouse position in both axis
	float CursorLocationX = XLocation / SizeX; // Gets the X cursor location on the viewport
	float CursorLocationY = YLocation / SizeY; // Gets the Y cursor location on the viewport
	if (CursorLocationX >= 0.975f) { // Checks whether X cursor location is on the right hand border
		EdgeMoveSpeedX = 5.0f; // If so, accelerate X edge movement speed
	}
	else {
		if (CursorLocationX <= 0.025f) { // Checks whether X cursor location is on the left hand border
			EdgeMoveSpeedX = -5.0f; // If so, reverse X edge movement speed
		}
		else { // Otherwise, do nothing
			EdgeMoveSpeedX = 0.0f;
		}
	}
	if (CursorLocationY >= 0.975f) { // Checks whether Y cursor location is on the lower border
		EdgeMoveSpeedY = -5.0f; // If so, reverse Y edge movement speed
	}
	else {
		if (CursorLocationY <= 0.025f) { // Checks whether Y cursor location is on the upper border
			EdgeMoveSpeedY = 5.0f; // If so, accelerate Y edge movement speed
		}
		else { // Otherwise, do nothing
			EdgeMoveSpeedY = 0.0f;
		}
	}

	DeltaXSpeed = EdgeMoveSpeedY * FastMoveMultiplier; // Calculates final values and the effect of the fast move multiplier
	DeltaYSpeed = EdgeMoveSpeedX * FastMoveMultiplier; // These values are then outputted as a function call and used to move pawn
}