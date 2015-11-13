// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "DeepIntoMeHUD.h"
#include "DeepIntoMePlayerController.generated.h"

UCLASS()
class ADeepIntoMePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetSpectatorMode();
	
	UFUNCTION(Client, Reliable)
	void ClientHUDStateChanged(EHUDState NewState);
};
