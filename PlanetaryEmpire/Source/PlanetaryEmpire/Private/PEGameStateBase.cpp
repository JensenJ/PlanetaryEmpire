// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameStateBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"

APEGameStateBase::APEGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void APEGameStateBase::BeginPlay() {
	GameTime.SetNum(3); // Limits size of array to 3
}

void APEGameStateBase::Tick(float DeltaSeconds) {
	SetClockwork(DeltaSeconds);
	Clock();
}

void APEGameStateBase::SetClockwork(float DeltaSeconds) {
	float DeltaTimeUnit = DeltaSeconds / TimeUnit;
	float AddedClockwork = DeltaTimeUnit + Clockwork;
	int NewDayTick = AddedClockwork / (60 * 24);
	float Remainder = FGenericPlatformMath::Fmod(AddedClockwork, (60.0f * 24.0f));
	Clockwork = Remainder;
	DayTick = NewDayTick;
}

void APEGameStateBase::Clock() {
	// Seconds
	float ClockworkSeconds = Clockwork * 3600;
	float NewSeconds = ClockworkSeconds / 60;
	float RemainderSeconds = FGenericPlatformMath::Fmod(NewSeconds, 60.0f);
	UKismetMathLibrary::FFloor(RemainderSeconds);
	Seconds = RemainderSeconds;

	// Minutes
	float NewMinutes = NewSeconds / 60;
	float RemainderMinutes = FGenericPlatformMath::Fmod(NewMinutes, 60.0f);
	UKismetMathLibrary::FFloor(RemainderMinutes);
	Minutes = RemainderMinutes;

	// Hours
	float NewHours = NewMinutes / 60;
	float RemainderHours = FGenericPlatformMath::Fmod(NewHours, 60.0f);
	UKismetMathLibrary::FFloor(RemainderHours);
	Hours = RemainderHours;

	GameTime.Insert(Seconds, 0);
	GameTime.Insert(Minutes, 1);
	GameTime.Insert(Hours, 2);

	FString strHours = FString::FromInt(GameTime[2]);
	FString HoursMinutesString = UKismetStringLibrary::BuildString_Int(strHours, ":", GameTime[1], "");
	FString FinalString = UKismetStringLibrary::BuildString_Int(HoursMinutesString, ":", GameTime[0], "");
	UE_LOG(LogTemp, Warning, TEXT("PEGameStateBase: Time: %s"), *FinalString);
}