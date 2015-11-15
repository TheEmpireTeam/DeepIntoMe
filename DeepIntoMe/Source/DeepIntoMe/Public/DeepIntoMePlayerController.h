// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "DeepIntoMeHUD.h"
#include "DeepIntoMePlayerController.generated.h"

UCLASS()
class ADeepIntoMePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	FTimerHandle RespawnTimer;
	
	const float SecondsToRespawn = 10.0f;
	
public:
	void SetSpectatorMode();
	
	UFUNCTION(Client, Reliable)
	void ClientHUDStateChanged(EHUDState NewState);
	
	UFUNCTION(Client, Reliable)
	void ClientStartRespawnTimer();
	
	void RespawnPlayer();
	
	UFUNCTION(Server, Reliable,WithValidation)
	void ServerRespawnPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	float GetSecondsToRespawn();
};
