// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "PECameraPawn.generated.h"

UCLASS()
class PLANETARYEMPIRE_API APECameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APECameraPawn(const FObjectInitializer& ObjectInit);

	// Getters for all camera components
	USceneComponent* GetSceneComponent();
	USphereComponent* GetSphereComponent();
	USpringArmComponent* GetSpringArmComponent();
	UCameraComponent* GetCameraComponent();

	// Camera Components in order of parentage
	UPROPERTY(EditAnywhere, Category = Camera)
	class USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CameraComponent = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
