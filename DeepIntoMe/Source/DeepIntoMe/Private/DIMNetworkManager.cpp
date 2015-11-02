// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMNetworkManager.h"

UDIMNetworkManager::UDIMNetworkManager()
: Super()
{
	LastPlayerIndex = 0;
}

int32 UDIMNetworkManager::RegisterPlayer(ADIMPlayerState* PlayerState, FString PlayerName)
{
	FPlayerNetworkInfo PlayerInfo;
	//PlayerInfo.NetworkId = LastPlayerIndex++;
	PlayerInfo.Nickname = PlayerName;
	PlayerInfo.Health = 100.0f;

	PlayersInfo.Add(PlayerInfo);

	return PlayerInfo.NetworkId;
}

FPlayerNetworkInfo UDIMNetworkManager::GetPlayerInfo(int32 PlayerIndex)
{
	if (PlayersInfo.Num() > PlayerIndex)
		return PlayersInfo[PlayerIndex];
	else
		return FPlayerNetworkInfo();
}

void UDIMNetworkManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
