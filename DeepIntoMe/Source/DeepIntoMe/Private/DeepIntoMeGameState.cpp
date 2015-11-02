// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeGameState.h"


void ADeepIntoMeGameState::BeginPlay()
{
	Super::BeginPlay();

	// Create network manager on server
	if (Role == ROLE_Authority)
		CreateNetworkManager();
}

void ADeepIntoMeGameState::CreateNetworkManager()
{
	NetworkManager = NewObject<UDIMNetworkManager>();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Network Manager Created!"));
}

UDIMNetworkManager* ADeepIntoMeGameState::GetNetworkManager()
{
	return NetworkManager;
}

void ADeepIntoMeGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
