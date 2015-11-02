// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"

void ADIMPlayerState::BeginPlay()
{
	Super::BeginPlay();

	ResetScore();

	const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
	PlayerName = FPlatformProcess::ComputerName() + Suffix;

	if (Role == ROLE_Authority)
		RegisterInNetworkManager();
}

void ADIMPlayerState::ResetScore()
{
	NumKills = 0;
	NumDeaths = 0;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, PlayerName + TEXT(": Score has been reset!"));
}

void ADIMPlayerState::RegisterInNetworkManager_Implementation()
{
	MultiplayerId = -1;

	ADeepIntoMeGameState * GameState = GetWorld()->GetGameState<ADeepIntoMeGameState>();
	if (GameState)
	{
		MultiplayerId = GameState->GetNetworkManager()->RegisterPlayer(this, PlayerName);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Player Registered!"));
	}
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
