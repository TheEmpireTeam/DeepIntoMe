// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"
#include "DeepIntoMeHUD.h"

void ADIMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
		SetPlayerName(FPlatformProcess::ComputerName() + Suffix);
	}
	//GiveName();

	ResetScore();
}

void ADIMPlayerState::ResetScore()
{
	NumKills = 0;
	NumDeaths = 0;
}

void ADIMPlayerState::GiveName()
{
	if (Role < ROLE_Authority)
		ServerGiveName();
	else
		GiveName();
}

void ADIMPlayerState::ServerGiveName_Implementation()
{
	GiveName();
}

bool ADIMPlayerState::ServerGiveName_Validate()
{
	return true;
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADIMPlayerState, NumKills);
	DOREPLIFETIME(ADIMPlayerState, NumDeaths);
}
