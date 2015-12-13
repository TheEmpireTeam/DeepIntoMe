// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"
#include "DIMPlayerState.h"
#include "DeepIntoMePlayerController.h"
#include "Weapon.h"


void ADeepIntoMeGameState::BeginPlay()
{
	Super::BeginPlay();
	
	ADIMGameMode* GameMode = Cast<ADIMGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		CurrentPlayMode = GameMode->GetDefaultPlayMode();
	}
	else
	{
		CurrentPlayMode = EGamePlayMode::Deathmatch;
	}
	
	ADeepIntoMeHUD* HUD = Cast<ADeepIntoMeHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD)
	{
		HUD->SetGamePlayMode(CurrentPlayMode);
	}
	
	//bGameStarted = false;
	if (Role == ROLE_Authority)
	{
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &ADeepIntoMeGameState::RebindPlayerWeapons, 2.0f, false);
	}
}

void ADeepIntoMeGameState::RebindPlayerWeapons()
{
	//bGameStarted = true;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(*Iterator);
		if (Controller)
		{
			AMainCharacter* Character = Cast<AMainCharacter>(Controller->GetPawn());
			if (Character)
			{
				Character->RebindWeapon();
			}
		}
	}
}

bool ADeepIntoMeGameState::GameStarted()
{
	return bGameStarted;
}

void ADeepIntoMeGameState::SpawnPlayerInventory(AMainCharacter* PlayerPawn)
{
	/*FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TSubclassOf<AWeapon> WeaponType = PlayerPawn->GetWeaponType();
	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponType, FVector(0, 0, 1000), FRotator(0, 0, 0), SpawnParameters);
	
	if (NewWeapon)
	{
		PlayerPawn->AddWeapon(NewWeapon);
	}*/
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

FString ADeepIntoMeGameState::GetPlayModeName()
{
	return ADIMGameMode::GamePlayModeToString(CurrentPlayMode);
}

EGamePlayMode ADeepIntoMeGameState::GetPlayMode()
{
	return CurrentPlayMode;
}

void ADeepIntoMeGameState::SetPlayMode(EGamePlayMode NewPlayMode)
{
	CurrentPlayMode = NewPlayMode;
	
	NetMulticastRefreshGamePlayMode(CurrentPlayMode);
	
	// Handle changes
}

void ADeepIntoMeGameState::NetMulticastRefreshGamePlayMode_Implementation(const EGamePlayMode NewPlayMode)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(*Iterator);
		if (Controller)
		{
			ADeepIntoMeHUD* HUD = Cast<ADeepIntoMeHUD>(Controller->GetHUD());
			if (HUD)
			{
				HUD->SetGamePlayMode(NewPlayMode);
			}
		}
	}
}
