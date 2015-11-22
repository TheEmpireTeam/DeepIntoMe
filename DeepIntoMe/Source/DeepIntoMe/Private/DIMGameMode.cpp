// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMGameMode.h"
#include "DIMPlayerStart.h"
#include "DIMPlayerState.h"
#include "DeepIntoMePlayerController.h"
#include "MainCharacter.h"

	
AActor* ADIMGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("ChoosePlayerStart"));

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
		return PreferredSpawns[FMath::RandRange(0, PreferredSpawns.Num() - 1)];
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

/*AActor* ADIMGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("FindPlayerStart"));
	return Super::FindPlayerStart(Player, IncomingName);
}*/


void ADIMGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	
	AMainCharacter* Pawn = Cast<AMainCharacter>(NewPlayer->GetPawn());
	if (Pawn)
	{
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
	
	AMainCharacter* Pawn = Cast<AMainCharacter>(NewPlayer->GetPawn());
	if (Pawn)
	{
		Pawn->ServerInvokeColorChange();
	}
}

