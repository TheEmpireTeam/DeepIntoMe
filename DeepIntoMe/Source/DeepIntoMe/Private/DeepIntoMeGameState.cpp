// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"


void ADeepIntoMeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

FPlayerInfo ADeepIntoMeGameState::GetPlayerInfo(int32 PlayerIndex)
{
	if (PlayersInfo.Num() > PlayerIndex)
		return PlayersInfo[PlayerIndex];
	else
		return FPlayerInfo();
}
