// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMePlayerController.h"


ADeepIntoMePlayerController::ADeepIntoMePlayerController()
: Super()
{
	SetPlayerName(TEXT("Default Name"));
}

void ADeepIntoMePlayerController::SetPlayerName(FString NewName)
{
	PlayerName = NewName;
}

FString ADeepIntoMePlayerController::GetPlayerName()
{
	return PlayerName;
}

/*void ADeepIntoMePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeepIntoMePlayerController, PlayerName);
}*/
