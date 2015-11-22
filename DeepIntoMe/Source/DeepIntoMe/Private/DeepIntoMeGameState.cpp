// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"
#include "DIMPlayerState.h"
#include "DeepIntoMePlayerController.h"
#include "MainCharacter.h"


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

void ADeepIntoMeGameState::ClientRepaintOtherPlayerPawns_Implementation()
{
	TArray<AActor*> PlayerPawns;
	UGameplayStatics::GetAllActorsOfClass(this, AMainCharacter::StaticClass(), PlayerPawns);

	for (int32 i = 0; i < PlayerPawns.Num(); i++)
	{
		AMainCharacter* PlayerPawn = Cast<AMainCharacter>(PlayerPawns[i]);
		if (PlayerPawn)
		{
			PlayerPawn->ServerInvokeColorChange();
		}
	}
}

void ADeepIntoMeGameState::RepaintOtherPlayerPawns()
{
	if (Role < ROLE_Authority)
	{
		ServerRepaintOtherPlayerPawns();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("RepaintOtherPlayerPawns"));
	
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(*Iterator);
			if (Controller)
			{
				AMainCharacter* PlayerPawn = Cast<AMainCharacter>(Controller->GetPawn());
				if (PlayerPawn)
				{
					PlayerPawn->ServerInvokeColorChange();
				}
			}
		}
	}
}

void ADeepIntoMeGameState::ServerRepaintOtherPlayerPawns_Implementation()
{
	RepaintOtherPlayerPawns();
}

bool ADeepIntoMeGameState::ServerRepaintOtherPlayerPawns_Validate()
{
	return true;
}
