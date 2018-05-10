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
	
	// Setup scene component
	SceneComponent->AttachTo(RootComponent);

	// Setup sphere component
	SphereComponent->AttachTo(SceneComponent);
	SphereComponent->RelativeLocation = FVector(0.0f, 0.0f, 50.0f);
	SphereComponent->RelativeRotation = FRotator(0.0f, 70.0f, 0.0f);

	// Setup spring arm component
	SpringArmComponent->AttachTo(SphereComponent);
	SpringArmComponent->bAbsoluteRotation = true;
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	SpringArmComponent->RelativeRotation = FRotator(0.0, 0.0f, 0.0f);

	// Setup camera component
	CameraComponent->AttachTo(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

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