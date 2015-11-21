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

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SecondsToRespawn;
	
public:

	ADeepIntoMePlayerController();
	
	virtual void BeginPlay() override;

	void SetSpectatorMode();
	
	UFUNCTION(Client, Reliable)
	void ClientHUDStateChanged(EHUDState NewState);
	
	UFUNCTION(Client, Reliable)
	void ClientStartRespawnTimer();
	
	void RespawnPlayer();
	
	UFUNCTION(Client, Reliable)
	void ClientShowKillMessage(const FString& KillerName, const FString& VictimName);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdatePawnColor();
	
	UFUNCTION(Server, Reliable,WithValidation)
	void ServerRespawnPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	float GetSecondsToRespawn();
	
	UFUNCTION(Client, Reliable)
	void ClientUpdatePlayersTeamColor();
	
	UFUNCTION(Client, Reliable)
	void ClientAddConsoleMessage(const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void BroadcastConsoleMessage(const FString& Message);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBroadcastConsoleMessage(const FString& Message);
};
