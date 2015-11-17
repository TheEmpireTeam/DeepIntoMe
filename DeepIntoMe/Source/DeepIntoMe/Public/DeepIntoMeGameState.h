// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "DIMPlayerState.h"
#include "DIMNetworkManager.h"
#include "DeepIntoMeGameState.generated.h"

UCLASS()
class ADeepIntoMeGameState : public AGameState
{
	GENERATED_BODY()

public:
	// Get team by automatic team filling mechanism
	int32 GetNextPlayerTeamNumber();
	
	UFUNCTION(BlueprintCallable, Category = "Statistics")
	TArray<APlayerState*> GetMankindPlayers();
	
	UFUNCTION(BlueprintCallable, Category = "Statistics")
	TArray<APlayerState*> GetBailoshPlayers();
	
	TArray<APlayerState*> GetPlayersOfTeam(int32 TeamNumber);
};
