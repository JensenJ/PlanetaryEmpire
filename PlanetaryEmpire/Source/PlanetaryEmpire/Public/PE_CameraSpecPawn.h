// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "PE_CameraSpecPawn.generated.h"

UCLASS()
class PLANETARYEMPIRE_API APE_CameraSpecPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	APE_CameraSpecPawn(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* CameraComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraXYLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraHeightMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraHeightMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraHeightAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraHeightAngleMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraHeightAngleMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraRadiusMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraRadiusMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraZoomSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraMovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float CameraScrollBoundary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		bool bCanMoveCamera;
private:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
public:
	void ZoomInByWheel();
	void ZoomOutByWheel();
	void RotateLeftByWheel();
	void RotateRightByWheel();
	void RotateUpByWheel();
	void RotateDownByWheel();
	void RepositionCamera();
private:
	float FastMoveValue;
	float RotateValue;
	float MoveForwardValue;
	float MoveRightValue;
	float MoveUpValue;
	float ZoomInValue;
public:
	void FastMoveInput(float Direction);
	void RotateInput(float Direction);
	void MoveCameraForwardInput(float Direction);
	void MoveCameraRightInput(float Direction);
	void MoveCameraUpInput(float Direction);
	void ZoomCameraInInput(float Direction);
private:
	FVector MoveCameraForward(float Direction);
	FVector MoveCameraRight(float Direction);
	FRotator GetIsolatedCameraYaw();
	float MoveCameraUp(float Direction);
	void ZoomCameraIn(float Direction);
	void TurnCameraUp(float Direction);
	void TurnCameraRight(float Direction);
public:
	virtual void Tick(float DeltaSeconds) override;
	float GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const;
};