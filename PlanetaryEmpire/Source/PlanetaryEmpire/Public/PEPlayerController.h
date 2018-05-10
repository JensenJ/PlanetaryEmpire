// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
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

	//Decides whether camera can move
	bool bCameraMoveable = true;

	//Input methods
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);
	void InputMoveCameraForward(float value);
	void InputMoveCameraRight(float value);
	void InputFastMoveCamera(float value);
	void InputRotateCamera(float value);
	void InputZoomInCamera(float value);

	void CalculateMovementSpeed();
};
