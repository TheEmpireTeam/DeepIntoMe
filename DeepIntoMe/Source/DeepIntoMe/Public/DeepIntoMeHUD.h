// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "DIMGameMode.h"
#include "DeepIntoMeHUD.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8 {
	MainMenu,
	Ingame,
	Spectator
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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUDEvents")
	void AddConsoleMessage(const FString& Message);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUDEvents")
	void ShowKillMessage(const FString& KillerName, const FString& VictimName);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUDEvents")
	void SetGamePlayMode(const EGamePlayMode NewMode);

	// Event hook to update HUD state (eg. to determine visibility of widgets)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUDEvents")
	void OnStateChanged(EHUDState NewState);
};
