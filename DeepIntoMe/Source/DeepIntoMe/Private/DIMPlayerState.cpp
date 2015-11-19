// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"
#include "DeepIntoMeHUD.h"
#include "MainCharacter.h"
#include "DeepIntoMePlayerController.h"


void ADIMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	ResetScore();		
	GiveName();
	
	AskTeamNumber();
	
	ADeepIntoMeGameState* GameState = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->ClientRepaintOtherPlayerPawns();
	}
}

void ADIMPlayerState::ResetScore()
{
	NumKills = 0;
	NumDeaths = 0;
}

void ADIMPlayerState::GiveName()
{
	if (Role < ROLE_Authority)
	{
		ServerGiveName();
	}
	else
	{
		const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
		SetPlayerName(FPlatformProcess::ComputerName() + Suffix);
	}
}

void ADIMPlayerState::ServerGiveName_Implementation()
{
	GiveName();
}

bool ADIMPlayerState::ServerGiveName_Validate()
{
	return true;
}

void ADIMPlayerState::OverrideName(const FString& Nickname)
{
	if (Role < ROLE_Authority)
	{
		UDIMGameInstance* GameInstance = Cast<UDIMGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance && !GameInstance->GetNickname().IsEmpty())
		{
			ServerOverrideName(GameInstance->GetNickname());
		}
	}
	else
	{
		SetPlayerName(Nickname);
	}
}

void ADIMPlayerState::ServerOverrideName_Implementation(const FString& Nickname)
{
	SetPlayerName(Nickname);
}

bool ADIMPlayerState::ServerOverrideName_Validate(const FString& Nickname)
{
	return true;
}

void ADIMPlayerState::AddKill()
{
	NumKills++;
}

void ADIMPlayerState::AddDeath()
{
	NumDeaths++;
}

void ADIMPlayerState::AskTeamNumber()
{
	// Ask player team on server side
	if (Role == ROLE_Authority)
	{
		ADeepIntoMeGameState* GS = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
		TeamNumber = GS->GetNextPlayerTeamNumber();
	}
}

void ADIMPlayerState::ServerAskTeamNumber_Implementation()
{
	AskTeamNumber();
}

bool ADIMPlayerState::ServerAskTeamNumber_Validate()
{
	return true;
}

int32 ADIMPlayerState::GetTeamNumber()
{
	return TeamNumber;
}

int32 ADIMPlayerState::GetKills()
{
	return NumKills;
}
	
int32 ADIMPlayerState::GetDeaths()
{
	return NumDeaths;
}

void ADIMPlayerState::UpdatePlayerPawnColor()
{
	ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(GetWorld()->GetFirstPlayerController());
	if (Controller)
	{
		AMainCharacter* PlayerPawn = Cast<AMainCharacter>(Controller->GetPawn());
		if (PlayerPawn)
		{
			PlayerPawn->ServerInvokeColorChange();
		}
	}
}

// Called, when team number update received from server
void ADIMPlayerState::OnRep_TeamNumber()
{
	UpdatePlayerPawnColor();
	
	ADeepIntoMeGameState* GameState = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->ClientRepaintOtherPlayerPawns();
	}
}

void ADIMPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}

void ADIMPlayerState::ServerSetTeamNumber_Implementation(int32 NewTeamNumber)
{
	SetTeamNumber(NewTeamNumber);
}

bool ADIMPlayerState::ServerSetTeamNumber_Validate(int32 NewTeamNumber)
{
	return true;
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADIMPlayerState, NumKills);
	DOREPLIFETIME(ADIMPlayerState, NumDeaths);
	DOREPLIFETIME(ADIMPlayerState, TeamNumber);
	//DOREPLIFETIME_CONDITION(ADIMPlayerState, TeamNumber, COND_SkipOwner);
}
