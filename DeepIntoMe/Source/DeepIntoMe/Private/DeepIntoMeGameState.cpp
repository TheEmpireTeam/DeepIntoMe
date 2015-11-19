// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"
#include "DIMPlayerState.h"
#include "DeepIntoMePlayerController.h"


int32 ADeepIntoMeGameState::GetNextPlayerTeamNumber()
{
	int32 MankindPlayers = 0, BarnyForcesPlayers = 0;

	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		ADIMPlayerState* PS = Cast<ADIMPlayerState>(PlayerArray[i]);
		if (PS)
		{
			switch (PS->GetTeamNumber())
			{
				case 0:
					MankindPlayers++;
					break;
				case 1:
					BarnyForcesPlayers++;
					break;
			}
		}
	}
	
	return (BarnyForcesPlayers < MankindPlayers) ? 1 : 0;
}

TArray<APlayerState*> ADeepIntoMeGameState::GetMankindPlayers()
{
	return GetPlayersOfTeam(0);
}
	
TArray<APlayerState*> ADeepIntoMeGameState::GetBailoshPlayers()
{
	return GetPlayersOfTeam(1);
}

TArray<APlayerState*> ADeepIntoMeGameState::GetPlayersOfTeam(int32 TeamNumber)
{
	TArray<APlayerState*> Players;
	
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		ADIMPlayerState* PS = Cast<ADIMPlayerState>(PlayerArray[i]);
		if (PS && PS->GetTeamNumber() == TeamNumber)
		{
			Players.Add(PS);
		}
	}
	
	return Players;
}

void ADeepIntoMeGameState::StartNight_Implementation()
{

}

// Function must be called on server in order to work correctly
void ADeepIntoMeGameState::BroadcastKillMessage(const FString& KillerName, const FString& VictimName)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(*Iterator);
		if (Controller)
		{
			Controller->ClientShowKillMessage(KillerName, VictimName);
		}
	}
}
