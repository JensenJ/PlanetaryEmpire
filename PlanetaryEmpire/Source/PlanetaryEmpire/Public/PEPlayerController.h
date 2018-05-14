// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "PECameraPawn.h"
#include "PEPlayerController.generated.h"

UCLASS()
class PLANETARYEMPIRE_API APEPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	APEPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Camera Components in order of parentage
	UPROPERTY(EditAnywhere, Category = Camera)
	class USceneComponent* LocalSceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USphereComponent* LocalSphereComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* LocalSpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* LocalCameraComponent = nullptr;

	// Reference to CameraPawn class
	APECameraPawn* CameraPawn = nullptr;

	// Function to get controlled pawn
	APECameraPawn* GetControlledPawn() const;

	// Input methods
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);
	void InputMoveCameraForward(float value);
	void InputMoveCameraRight(float value);
	void InputCameraX(float value);
	void InputCameraY(float value);
	void InputTogglePan();
	void InputToggleFastMove();
	void InputZoomIn();
	void InputZoomOut();

	// Camera Functions
	float CalculateMovementSpeed();
	FTransform MovementX(float AxisValue, float MoveSensitivity, float SpeedMultiplier);
	FTransform MovementY(float AxisValue, float MoveSensitivity, float SpeedMultiplier);
	FRotator PanX(float AxisValue, float PanSensitiviy);
	FRotator PanY(float AxisValue, float PanSensitivity);
	void ScreenEdgeMovement(float AxisValue, float &DeltaXSpeed, float &DeltaYSpeed);

	//Camera Variables
	bool bCameraMoveable = true;
	bool bPanToggled = false;
	bool bFastMove = false;
	float MovementSpeed = 0.0f;
	float FastMoveMultiplier = 1.0f;
	float PanSensitivity = 5.0f;
	float ZoomSensitivity = 50.0f;
	float MinArmDistance = 500.0f;
	float MaxArmDistance = 2500.0f;
	float EdgeMoveSpeedX = 0.0f;
	float EdgeMoveSpeedY = 0.0f;
};