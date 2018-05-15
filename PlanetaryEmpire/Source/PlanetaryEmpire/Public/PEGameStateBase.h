// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PEGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PLANETARYEMPIRE_API APEGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	APEGameStateBase();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	// Clock Functions
	void SetClockwork(float DeltaSeconds);
	void Clock();
	// Clock Variables
	float TimeUnit = 1.0f;
	float Clockwork = 0.0f;
	float DayTick = 0;
	int32 Seconds = 0;
	int32 Minutes = 0;
	int32 Hours = 0;
	int32 Day = 1;
	int32 Month = 1;
	int32 Year = 1;
	TArray<int32> GameTime;
	TArray<int32> GameDate;
};