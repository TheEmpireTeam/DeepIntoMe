// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMePlayerController.h"


void ADeepIntoMePlayerController::SetSpectatorMode()
{
	PlayerState->bIsSpectator = true;
	ChangeState(NAME_Spectating);
	ClientGotoState(NAME_Spectating);
	
	ClientHUDStateChanged(EHUDState::Spectator);
}

void ADeepIntoMePlayerController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	ADeepIntoMeHUD* HUD = Cast<ADeepIntoMeHUD>(GetHUD());
	if (HUD)
	{
		HUD->OnStateChanged(NewState);
	}
}
