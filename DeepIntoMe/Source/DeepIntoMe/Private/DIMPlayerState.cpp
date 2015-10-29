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
}

void ADIMPlayerState::ResetScore()
{
	NumKills = 0;
	NumDeaths = 0;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("ResetScore()"));
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
