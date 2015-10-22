// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "DeepIntoMeGameMode.generated.h"


USTRUCT()
struct PlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Nickname;

public:
	PlayerInfo();
};

UCLASS()
class DEEPINTOME_API ADeepIntoMeGameMode : public AGameMode
{
	GENERATED_BODY()

	ADeepIntoMeGameMode(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(Reliable, Replicated, NetMulticast)
	TArray<PlayerInfo> ConnectedPlayers;

};
