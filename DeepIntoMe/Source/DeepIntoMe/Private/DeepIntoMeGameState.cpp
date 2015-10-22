// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"


int32 ADeepIntoMeGameState::AddPlayer(FString PlayerName)
{
	FPlayerInfo NewPlayerInfo =
	{
		PlayerName
	};

	return ConnectedPlayers.Add(NewPlayerInfo);
}

void ADeepIntoMeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
