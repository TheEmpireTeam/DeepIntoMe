// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"
#include "DeepIntoMeHUD.h"

void ADIMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Force server to give player a name
	if (Role == ROLE_Authority)
	{
		const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
		SetPlayerName(FPlatformProcess::ComputerName() + Suffix);
	}

	ResetScore();
	//RegisterInNetworkManager();
}

void ADIMPlayerState::ResetScore()
{
	NumKills = 0;
	NumDeaths = 0;
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

bool ADIMPlayerState::RegisterInNetworkManager_Validate()
{
	return true;
}

void ADIMPlayerState::OnRep_MultiplayerId(int32 PreviousId)
{
	int32 some_int = PreviousId;
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADIMPlayerState, MultiplayerId);
	DOREPLIFETIME(ADIMPlayerState, NumKills);
	DOREPLIFETIME(ADIMPlayerState, NumDeaths);
}
