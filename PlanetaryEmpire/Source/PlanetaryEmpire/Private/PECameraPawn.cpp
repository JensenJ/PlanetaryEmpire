// Fill out your copyright notice in the Description page of Project Settings.

#include "PECameraPawn.h"


// Sets default values
APECameraPawn::APECameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

