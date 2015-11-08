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
	virtual void BeginPlay() override;
};
