// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMePlayerController.h"
#include "DIMPlayerStart.h"
#include "MainCharacter.h"
#include "DIMGameMode.h"
#include "DeepIntoMeGameState.h"


ADeepIntoMePlayerController::ADeepIntoMePlayerController()
{
	SecondsToRespawn = 10.0f;
}

void ADeepIntoMePlayerController::BeginPlay()
{
	// Load player pawn colors on start
	//ClientUpdatePlayersTeamColor();
}

void ADeepIntoMePlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	
	AMainCharacter* Character = Cast<AMainCharacter>(InPawn);
	if (Character)
	{
		Character->RebindWeapon();
	}
}

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

void ADeepIntoMePlayerController::ClientShowKillMessage_Implementation(const FString& KillerName, const FString& VictimName)
{
	ADeepIntoMeHUD* HUD = Cast<ADeepIntoMeHUD>(GetHUD());
	if (HUD)
	{
		HUD->ShowKillMessage(KillerName, VictimName);
	}
}

void ADeepIntoMePlayerController::ClientUpdatePawnColor_Implementation()
{
	/*AMainCharacter* PlayerPawn = Cast<AMainCharacter>(GetPawn());
	if (PlayerPawn)
	{
		PlayerPawn->UpdateTeamColor();
	}*/
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

/*void ADeepIntoMePlayerController::ServerRestartPlayer_Implementation()
{
	Super::ServerRestartPlayer();
	
	ADIMGameMode* GameMode = Cast<ADIMGameMode>(UGameplayStatics::GetGameMode(GetWorld()));		
	GameMode->RestartPlayer(this);
		
	ClientHUDStateChanged(EHUDState::Ingame);
}*/

float ADeepIntoMePlayerController::GetSecondsToRespawn()
{
	return (SecondsToRespawn - GetWorldTimerManager().GetTimerElapsed(RespawnTimer));
}

void ADeepIntoMePlayerController::ClientUpdatePlayersTeamColor_Implementation()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("ClientUpdatePlayersTeamColor"));

	TArray<AActor*> PlayerPawns;
	UGameplayStatics::GetAllActorsOfClass(this, AMainCharacter::StaticClass(), PlayerPawns);

	// Split the player starts into two arrays for preferred and fallback spawns
	for (int32 i = 0; i < PlayerPawns.Num(); i++)
	{
		AMainCharacter* PlayerPawn = Cast<AMainCharacter>(PlayerPawns[i]);
		if (PlayerPawn)
		{
			PlayerPawn->UpdateTeamColor();
		}
	}*/
}

void ADeepIntoMePlayerController::ClientAddConsoleMessage_Implementation(const FString& Message)
{
	ADeepIntoMeHUD* HUD = Cast<ADeepIntoMeHUD>(GetHUD());
	if (HUD)
	{
		HUD->AddConsoleMessage(Message);
	}
}

void ADeepIntoMePlayerController::BroadcastConsoleMessage(const FString& Message)
{
	if (Role < ROLE_Authority)
	{
		ServerBroadcastConsoleMessage(Message);
	}
	else
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(*Iterator);
			if (Controller)
			{
				Controller->ClientAddConsoleMessage(Message);
			}
		}
	}
}

void ADeepIntoMePlayerController::ServerBroadcastConsoleMessage_Implementation(const FString& Message)
{
	BroadcastConsoleMessage(Message);
}

bool ADeepIntoMePlayerController::ServerBroadcastConsoleMessage_Validate(const FString& Message)
{
	return true;
}
