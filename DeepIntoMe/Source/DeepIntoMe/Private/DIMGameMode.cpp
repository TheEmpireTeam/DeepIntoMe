// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMGameMode.h"
#include "DIMPlayerStart.h"
#include "DIMPlayerState.h"
#include "DeepIntoMePlayerController.h"
#include "MainCharacter.h"

	
AActor* ADIMGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<ADIMPlayerStart*> PreferredSpawns;

	// Get all playerstart objects in level
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, ADIMPlayerStart::StaticClass(), PlayerStarts);

	// Split the player starts into two arrays for preferred and fallback spawns
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		ADIMPlayerStart* TestStart = Cast<ADIMPlayerStart>(PlayerStarts[i]);
		if (TestStart)
		{
			ADIMPlayerState* PS = Cast<ADIMPlayerState>(Player->PlayerState);
			if (PS && PS->GetTeamNumber() == TestStart->TeamNumber)
			{
				PreferredSpawns.Add(TestStart);
			}
		}
	}
	
	if (PreferredSpawns.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PreferredSpawns.Num() - 1);
		return PreferredSpawns[RandomIndex];
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
		AActor* SpawnPoint = ChoosePlayerStart(NewPlayer);
		if (SpawnPoint)
		{
			Pawn->SetActorLocation(SpawnPoint->GetActorLocation());
			Pawn->SetActorRotation(SpawnPoint->GetActorRotation());
		}
		
		Pawn->ServerInvokeColorChange();
	}
}

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
