// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "DeepIntoMeHUD.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8 {
	MainMenu,
	Ingame
};

UCLASS()
class ADeepIntoMeHUD : public AHUD
{
	GENERATED_BODY()

	ADeepIntoMeHUD(const FObjectInitializer& ObjectInitializer);

	EHUDState CurrentState;

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState();

	/* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION(BlueprintNativeEvent, Category = "HUDEvents")
	void OnStateChanged(EHUDState NewState);
};
