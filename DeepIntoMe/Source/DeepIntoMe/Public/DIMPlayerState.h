// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "DIMPlayerState.generated.h"

UCLASS()
class ADIMPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	int32 NumKills;

	UPROPERTY(Replicated)
	int32 NumDeaths;
	
	UPROPERTY(Replicated)
	int32 TeamNumber;

public:
	virtual void BeginPlay() override;

	void ResetScore();
	
	void GiveName();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGiveName();
	
	void AskTeamNumber();
	
	// Request team number from server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAskTeamNumber();
	
	UFUNCTION(BlueprintCallable, Category = "Info")
	int32 GetTeamNumber();
};
