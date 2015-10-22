// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "DeepIntoMeGameState.generated.h"

USTRUCT()
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Nickname;
};

UCLASS()
class ADeepIntoMeGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	TArray<FPlayerInfo> ConnectedPlayers;

	UFUNCTION()
	int32 AddPlayer(FString PlayerName);
};
