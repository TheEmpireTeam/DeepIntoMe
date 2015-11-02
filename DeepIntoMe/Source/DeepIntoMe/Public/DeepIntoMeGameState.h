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

	UPROPERTY(Replicated)
	UDIMNetworkManager* NetworkManager;

	UFUNCTION()
	void CreateNetworkManager();

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Game Statistics")
	UDIMNetworkManager* GetNetworkManager();
};
