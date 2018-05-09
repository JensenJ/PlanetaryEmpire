// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "PEPlayerController.generated.h"

/**
 * 
 */
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
	//CameraPawn Reference and getter
	UPROPERTY(VisibleAnywhere, Category = References)
	class APawn* CameraPawnRef = nullptr;
	APawn* GetCameraPawnReference() const;

	//Input methods
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent);
	void InputMoveCameraForward();
	void InputMoveCameraRight();
	void InputFastMoveCamera();
	void InputRotateCamera();
	void InputZoomInCamera();
};
