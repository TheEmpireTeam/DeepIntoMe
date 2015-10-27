// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"

ADIMPlayerState::ADIMPlayerState()
: Super()
{
	const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
	PlayerName = FPlatformProcess::ComputerName() + Suffix;
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
