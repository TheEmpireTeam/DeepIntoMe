// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "DIMPlayerState.generated.h"

UCLASS()
class ADIMPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 NumKills;

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 NumDeaths;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MultiplayerId)
	int32 MultiplayerId;

	UFUNCTION()
	void OnRep_MultiplayerId(int32 PreviousId);

public:
	virtual void BeginPlay() override;

	void ResetScore();

	UFUNCTION(Server, Reliable, WithValidation)
	void RegisterInNetworkManager();
};
