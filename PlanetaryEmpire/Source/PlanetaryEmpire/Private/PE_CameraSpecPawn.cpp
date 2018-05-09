// Fill out your copyright notice in the Description page of Project Settings.

#include "PE_CameraSpecPawn.h"
#include "Engine/GameViewportClient.h"
#include "Public/CollisionQueryParams.h"
#include "GameFramework/DefaultPawn.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

APE_CameraSpecPawn::APE_CameraSpecPawn(const FObjectInitializer& ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;

	bAddDefaultMovementBindings = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCollisionComponent()->bGenerateOverlapEvents = false;
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCollisionComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCollisionComponent()->SetSimulatePhysics(false);

	CameraXYLimit = 25000.f;
	CameraHeight = 1000.f;
	CameraHeightMin = 300.f;
	CameraHeightMax = 5000.f;
	CameraRadius = 2000.f;
	CameraRadiusMin = 1000.f;
	CameraRadiusMax = 8000.f;
	CameraZAngle = 0.f;
	CameraHeightAngle = 30.f;
	CameraHeightAngleMin = 15.f;
	CameraHeightAngleMax = 60.f;
	CameraZoomSpeed = 200.f;
	CameraRotationSpeed = 10.f;
	CameraMovementSpeed = 3000.f;
	CameraScrollBoundary = 25.f;
	bCanMoveCamera = true;

	if (!CameraComponent) return; 
	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RTS Camera"));
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = false;

	RepositionCamera();
}

void APE_CameraSpecPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	if (!PlayerInputComponent) return;

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &APE_CameraSpecPawn::ZoomOutByWheel);
	PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &APE_CameraSpecPawn::ZoomInByWheel);
	PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &APE_CameraSpecPawn::RotateUpByWheel);
	PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &APE_CameraSpecPawn::RotateDownByWheel);

	PlayerInputComponent->BindAxis("MoveForward", this, &APE_CameraSpecPawn::MoveCameraForwardInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APE_CameraSpecPawn::MoveCameraRightInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &APE_CameraSpecPawn::MoveCameraUpInput);
	PlayerInputComponent->BindAxis("ZoomIn", this, &APE_CameraSpecPawn::ZoomCameraInInput);

	PlayerInputComponent->BindAxis("FastMove", this, &APE_CameraSpecPawn::FastMoveInput);
	PlayerInputComponent->BindAxis("Rotate", this, &APE_CameraSpecPawn::RotateInput);
}

void APE_CameraSpecPawn::ZoomInByWheel() {
	if (!bCanMoveCamera) return;
	CameraRadius -= CameraZoomSpeed * FastMoveValue;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void APE_CameraSpecPawn::ZoomOutByWheel() {
	if (!bCanMoveCamera) return;
	CameraRadius += CameraZoomSpeed * FastMoveValue;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void APE_CameraSpecPawn::RotateLeftByWheel() {
	if (!bCanMoveCamera) return;
	CameraZAngle -= CameraRotationSpeed * FastMoveValue;
}

void APE_CameraSpecPawn::RotateRightByWheel() {
	if (!bCanMoveCamera) return;
	CameraZAngle += CameraRotationSpeed * FastMoveValue;
}

void APE_CameraSpecPawn::RotateUpByWheel() {
	if (!bCanMoveCamera) return;
	CameraHeightAngle += CameraRotationSpeed * FastMoveValue;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void APE_CameraSpecPawn::RotateDownByWheel() {
	if (!bCanMoveCamera) return;
	CameraHeightAngle -= CameraRotationSpeed * FastMoveValue;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void APE_CameraSpecPawn::RepositionCamera() {
	FVector NewLocation(0.f, 0.f, 0.f);
	FRotator NewRotation(0.f, 0.f, 0.f);

	float sinCameraZAngle = FMath::Sin(FMath::DegreesToRadians(CameraZAngle));
	float cosCameraZAngle = FMath::Cos(FMath::DegreesToRadians(CameraZAngle));
	float sinCameraHeightAngle = FMath::Sin(FMath::DegreesToRadians(CameraHeightAngle));
	float cosCameraHeightAngle = FMath::Cos(FMath::DegreesToRadians(CameraHeightAngle));

	NewLocation.X = cosCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Y = sinCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Z = sinCameraHeightAngle * CameraRadius;

	float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(GetActorLocation().X + NewLocation.X, GetActorLocation().Y + NewLocation.Y);
	if (GetActorLocation().Z + NewLocation.Z < TerrainSurfaceZ + CameraHeight) {
		NewLocation.Z = TerrainSurfaceZ - GetActorLocation().Z + CameraHeight;
	}
	CameraComponent->SetRelativeLocation(NewLocation);
	NewRotation = (FVector(0.0f, 0.0f, 0.0f) - NewLocation).Rotation();
	CameraComponent->SetRelativeRotation(NewRotation);
}

void APE_CameraSpecPawn::FastMoveInput(float Direction) {
	if (!bCanMoveCamera) return;
	FastMoveValue = FMath::Abs(Direction) * 2.0f;
	if (FastMoveValue == 0.0f) {
		FastMoveValue = 1.0f;
	}
}

void APE_CameraSpecPawn::RotateInput(float Direction) {
	if (!bCanMoveCamera) return;
	RotateValue = FMath::Abs(Direction);
}

void APE_CameraSpecPawn::MoveCameraForwardInput(float Direction) {
	if (!bCanMoveCamera) return;
	MoveForwardValue = Direction;
}

void APE_CameraSpecPawn::MoveCameraRightInput(float Direction) {
	if (!bCanMoveCamera) return;
	MoveRightValue = Direction;
}

void APE_CameraSpecPawn::MoveCameraUpInput(float Direction) {
	if (!bCanMoveCamera) return;
	MoveUpValue = Direction;
}

void APE_CameraSpecPawn::ZoomCameraInInput(float Direction) {
	if (!bCanMoveCamera) return;
	ZoomInValue = Direction;
}

FVector APE_CameraSpecPawn::MoveCameraForward(float Direction) {
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * GetIsolatedCameraYaw().Vector();
	return DeltaMovement;
}

FVector APE_CameraSpecPawn::MoveCameraRight(float Direction) {
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * (FRotator(0.0f, 90.0f, 0.0f) + GetIsolatedCameraYaw()).Vector();
	return DeltaMovement;
}

FRotator APE_CameraSpecPawn::GetIsolatedCameraYaw() {
	float IsolatedYaw = CameraComponent->GetComponentToWorld().Rotator().Yaw;
	return FRotator(0.0f, IsolatedYaw, 0.0f);
}

float APE_CameraSpecPawn::MoveCameraUp(float Direction) {
	float MovementValue = Direction * CameraMovementSpeed;
	return MovementValue;
}

void APE_CameraSpecPawn::ZoomCameraIn(float Direction) {
	float MovementValue = Direction * CameraMovementSpeed;
	CameraRadius += MovementValue;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void APE_CameraSpecPawn::TurnCameraUp(float Direction) {
	CameraHeightAngle -= Direction * CameraRotationSpeed * 10.0f;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void APE_CameraSpecPawn::TurnCameraRight(float Direction) {
	CameraZAngle += Direction * CameraRotationSpeed * 10.0f;
}

void APE_CameraSpecPawn::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	FVector2D MousePosition;
	FVector2D ViewportSize;

	UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();

	if (!GameViewport) return;
	GameViewport->GetViewportSize(ViewportSize);

	if (GameViewport->IsFocused(GameViewport->Viewport)
		&& GameViewport->GetMousePosition(MousePosition) && bCanMoveCamera) {
		UE_LOG(LogTemp, Warning, TEXT("Tick"))
			if (MousePosition.X < CameraScrollBoundary) {
				MoveRightValue = -1.0f;
			}
			else if (ViewportSize.X - MousePosition.X < CameraScrollBoundary) {
				MoveRightValue = 1.0f;
			}
			if (MousePosition.Y < CameraScrollBoundary) {
				MoveForwardValue = 1.0f;
			}
			else if (ViewportSize.Y - MousePosition.Y < CameraScrollBoundary) {
				MoveForwardValue = -1.0f;
			}

			FVector ActualLocation = GetActorLocation();
			FVector ActualMovement = FVector::ZeroVector;
			if (!RotateValue) return;
			if (RotateValue == 0.f) {
				ActualMovement += MoveCameraForward(MoveForwardValue * FastMoveValue * DeltaSeconds);
				ActualMovement += MoveCameraRight(MoveRightValue * FastMoveValue * DeltaSeconds);
			}
			ActualLocation += ActualMovement;

			CameraHeight += MoveCameraUp(MoveUpValue * FastMoveValue * DeltaSeconds);
			CameraHeight = FMath::Clamp(CameraHeight, CameraHeightMin, CameraHeightMax);
			float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(ActualLocation.X, ActualLocation.Y);
			ActualLocation.Z = TerrainSurfaceZ + CameraHeight;
			ActualLocation.X = FMath::Clamp(ActualLocation.X, -CameraXYLimit, CameraXYLimit);
			ActualLocation.Y = FMath::Clamp(ActualLocation.Y, -CameraXYLimit, CameraXYLimit);
			SetActorLocation(ActualLocation);

			if (RotateValue != 0.f) {
				TurnCameraUp(MoveForwardValue * FastMoveValue * DeltaSeconds);
				TurnCameraRight(MoveRightValue * FastMoveValue * DeltaSeconds);
			}

			ZoomCameraIn(ZoomInValue * FastMoveValue * DeltaSeconds);
			RepositionCamera();
	}
}

float APE_CameraSpecPawn::GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const {
	FCollisionQueryParams TraceParams(FName(TEXT("Terrain")));
	TraceParams.bTraceComplex = false;
	TraceParams.AddIgnoredActor(this);
	TraceParams.bFindInitialOverlaps = false;

	FHitResult Hit;

	FVector Start = FVector(XCoord, YCoord, GetActorLocation().Z + CameraRadius);
	FVector End = FVector(XCoord, YCoord, -500.f);

	// ECC_ channels should be set properly !!!
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, TraceParams);

	if (bHit) {
		return Hit.ImpactPoint.Z;
	}

	return 0.f;
}