// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.generated.h"

USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString Nickname;

	UPROPERTY()
	float Health;
};

UCLASS()
class ADeepIntoMeGameState : public AGameState
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	TArray<FPlayerInfo> PlayersInfo;

	UFUNCTION()
	FPlayerInfo GetPlayerInfo(int32 PlayerIndex);
};
