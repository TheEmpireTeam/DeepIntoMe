// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMGameMode.h"
#include "DIMPlayerStart.h"
#include "DIMPlayerState.h"
#include "DeepIntoMePlayerController.h"
#include "MainCharacter.h"
#include "DeepIntoMeGameState.h"
	
ADIMGameMode::ADIMGameMode()
: Super()
{
	DefaultPlayMode = EGamePlayMode::Deathmatch;
}
	
AActor* ADIMGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawns;

	// Get all playerstart objects in level
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	// Split the player starts into two arrays for preferred and fallback spawns
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		ADIMPlayerStart* TestStart = Cast<ADIMPlayerStart>(PlayerStarts[i]);
		if (TestStart)
		{
			ADIMPlayerState* PS = Cast<ADIMPlayerState>(Player->PlayerState);
			if (PS && PS->GetTeam() == TestStart->OwnerTeam)
			{
				PreferredSpawns.Add(TestStart);
			}
		}
	}
	
	if (PreferredSpawns.Num() > 0)
	{
		return PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ADIMGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	
	// Repaint your respawned pawn on all clients
	AMainCharacter* Pawn = Cast<AMainCharacter>(NewPlayer->GetPawn());
	if (Pawn)
	{
		ADeepIntoMeGameState* GameState = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
		if (GameState->GetPlayMode() != EGamePlayMode::Deathmatch)
		{
			Pawn->ServerInvokeColorChange();
		}
	}
}

/*bool ADIMGameMode::CanDealDamage(class ADIMPlayerState* DamageCauser, class ADIMPlayerState* DamagedPlayer) const
{
	return true;
}*/

FString ADIMGameMode::InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal)
{
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void ADIMGameMode::StartNewPlayer(APlayerController* NewPlayer)
{
	Super::StartNewPlayer(NewPlayer);
}

void ADIMGameMode::StartMatch()
{
	Super::StartMatch();
}

bool ADIMGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	/* Always pick a random location */
	return false;
}

void ADIMGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ADeepIntoMeGameState* GameState = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->InvokeWeaponRebindingDelay();
	}
}

EGamePlayMode ADIMGameMode::GetDefaultPlayMode()
{
	return DefaultPlayMode;
}

FString ADIMGameMode::GamePlayModeToString(EGamePlayMode Mode)
{
	switch (Mode)
	{
		case EGamePlayMode::Deathmatch:
			return TEXT("Deathmatch");
		case EGamePlayMode::TeamDeathmatch:
			return TEXT("Team Deathmatch");
		case EGamePlayMode::Dominate:
			return TEXT("Dominate");
		case EGamePlayMode::CaptureTheFlag:
			return TEXT("Capture The Flag");
	}
	
	return TEXT("");
}
