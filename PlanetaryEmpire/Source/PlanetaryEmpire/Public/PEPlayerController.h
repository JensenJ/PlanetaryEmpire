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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	// Decides whether camera can move
	bool bCameraMoveable = true;

	// Input methods
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);
	void InputMoveCameraForward(float value);
	void InputMoveCameraRight(float value);
	void InputFastMoveCamera(float value);
	void InputRotateCamera(float value);
	void InputZoomInCamera(float value);

	// Camera Functions
	float CalculateMovementSpeed();
	FTransform MovementX(float AxisValue, float MoveSensitivity, float SpeedMultiplier);

	//Camera Variables
	float MovementSpeed = 0.0f;
	float FastMoveMultiplier = 1.0f;
};