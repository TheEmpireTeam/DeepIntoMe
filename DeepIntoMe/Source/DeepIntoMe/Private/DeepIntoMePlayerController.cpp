// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMePlayerController.h"
#include "DIMPlayerStart.h"
#include "MainCharacter.h"
#include "DIMGameMode.h"


void ADeepIntoMePlayerController::SetSpectatorMode()
{
	PlayerState->bIsSpectator = true;
	ChangeState(NAME_Spectating);
	ClientGotoState(NAME_Spectating);
	
	ClientHUDStateChanged(EHUDState::Spectator);
	ClientStartRespawnTimer();
}

void ADeepIntoMePlayerController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	ADeepIntoMeHUD* HUD = Cast<ADeepIntoMeHUD>(GetHUD());
	if (HUD)
	{
		HUD->OnStateChanged(NewState);
	}
}

void ADeepIntoMePlayerController::ClientStartRespawnTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ADeepIntoMePlayerController::RespawnPlayer, SecondsToRespawn, false);
}

void ADeepIntoMePlayerController::RespawnPlayer()
{
	if (Role < ROLE_Authority)
	{
		ServerRespawnPlayer();
	}
	else
	{
		ADIMGameMode* GameMode = Cast<ADIMGameMode>(UGameplayStatics::GetGameMode(GetWorld()));		
		GameMode->RestartPlayer(this);
		
		ClientHUDStateChanged(EHUDState::Ingame);
	}
}

void ADeepIntoMePlayerController::ServerRespawnPlayer_Implementation()
{
	RespawnPlayer();
}

bool ADeepIntoMePlayerController::ServerRespawnPlayer_Validate()
{
	return true;
}

float ADeepIntoMePlayerController::GetSecondsToRespawn()
{
	return (SecondsToRespawn - GetWorldTimerManager().GetTimerElapsed(RespawnTimer));
}
