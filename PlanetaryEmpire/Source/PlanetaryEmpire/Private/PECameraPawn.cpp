// Fill out your copyright notice in the Description page of Project Settings.

#include "PECameraPawn.h"
#include "UObject/UObjectGlobals.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
APECameraPawn::APECameraPawn(const class FObjectInitializer& ObjectInit)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	///////////////////////////////////////////////////
	//////////// Camera Component Setup ///////////////
	///////////////////////////////////////////////////

	// Set controller rotator variables
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Instantiate all components
	SceneComponent = ObjectInit.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"), false);
	SphereComponent = ObjectInit.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComponent"), false);
	SpringArmComponent = ObjectInit.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArmComponent"), false);
	CameraComponent = ObjectInit.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraComponent"), false);
	
	if (!SceneComponent) {
		UE_LOG(LogTemp, Error, TEXT("PECameraPawn: Scene Component not found::PointerProtection"));
		return;
	}
	if (!SphereComponent) {
		UE_LOG(LogTemp, Error, TEXT("PECameraPawn: Sphere Component not found::PointerProtection"));
		return;
	}
	if (!SpringArmComponent) {
		UE_LOG(LogTemp, Error, TEXT("PECameraPawn: Spring Arm Component not found::PointerProtection"));
		return;
	}
	if (!CameraComponent) {
		UE_LOG(LogTemp, Error, TEXT("PECameraPawn: Camera Component not found::PointerProtection"));
		return;
	}

	// Setup scene component
	SceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SceneComponent->RelativeLocation = FVector(0.0f, 0.0f, 300.0f);

	// Setup sphere component
	SphereComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	SphereComponent->RelativeRotation = FRotator(-70.0f, 0.0f, 0.0f);

	// Setup spring arm component
	SpringArmComponent->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->bAbsoluteRotation = true;
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	SpringArmComponent->RelativeRotation = FRotator(0.0, 0.0f, 0.0f);

	// Setup camera component
	CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	SphereComponent->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	///////////////////////////////////////////////////
}

// Called when the game starts or when spawned
void APECameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APECameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APECameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Getters
USceneComponent* APECameraPawn::GetSceneComponent() {
	return CameraComponent;
}
USphereComponent* APECameraPawn::GetSphereComponent() {
	return SphereComponent;
}
USpringArmComponent* APECameraPawn::GetSpringArmComponent() {
	return SpringArmComponent;
}
UCameraComponent* APECameraPawn::GetCameraComponent() {
	return CameraComponent;
}