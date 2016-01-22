// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "DIMPlayerState.h"
#include "DIMGameMode.h"
#include "MainCharacter.h"
#include "DeepIntoMeGameState.generated.h"

UCLASS()
class ADeepIntoMeGameState : public AGameState
{
	GENERATED_BODY()
	
private:
	EGamePlayMode CurrentPlayMode;
	
	bool bGameStarted;

public:
	virtual void BeginPlay() override;

	// Get team by automatic team filling mechanism
	EMultiplayerTeam GetNextPlayerTeam();
	
	UFUNCTION(BlueprintCallable, Category = "Statistics")
	TArray<APlayerState*> GetMankindPlayers();
	
	UFUNCTION(BlueprintCallable, Category = "Statistics")
	TArray<APlayerState*> GetBailoshPlayers();
	
	TArray<APlayerState*> GetPlayersOfTeam(EMultiplayerTeam Team);
	
	void BroadcastKillMessage(const FString& KillerName, const FString& VictimName);
	
	UFUNCTION(Client, Reliable)
	void ClientRepaintOtherPlayerPawns();
	
	void RepaintOtherPlayerPawns();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRepaintOtherPlayerPawns();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	FString GetPlayModeName();
	
	void SetPlayMode(EGamePlayMode NewPlayMode);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	EGamePlayMode GetPlayMode();
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRefreshGamePlayMode(const EGamePlayMode NewPlayMode);
	
	void SpawnPlayerInventory(AMainCharacter* PlayerPawn);
	
	void RebindPlayerWeapons();
	
	bool GameStarted();
	
	void InvokeWeaponRebindingDelay();
};
