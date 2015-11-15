// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"
#include "DIMPlayerState.h"


void ADeepIntoMeGameState::BeginPlay()
{
	Super::BeginPlay();
}

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
