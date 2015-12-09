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
	
	UPROPERTY(ReplicatedUsing = OnRep_TeamNumber)
	int32 TeamNumber;
	
	bool bIsDead;

public:
	virtual void BeginPlay() override;

	void ResetScore();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerResetScore();
	
	void GiveName();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerGiveName();
	
	void OverrideName(const FString& Nickname);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOverrideName(const FString& Nickname);
	
	void AskTeamNumber();

	void AddKill();
	
	void AddDeath();
	
	void SetTeamNumber(int32 NewTeamNumber);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetTeamNumber(int32 NewTeamNumber);
	
	// Request team number from server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAskTeamNumber();
	
	UFUNCTION(BlueprintCallable, Category = "Info")
	int32 GetTeamNumber();
	
	UFUNCTION(BlueprintCallable, Category = "Info")
	int32 GetKills();
	
	UFUNCTION(BlueprintCallable, Category = "Info")
	int32 GetDeaths();
	
	void UpdatePlayerPawnColor();
	
	UFUNCTION()
	void OnRep_TeamNumber();
	
	UFUNCTION(BlueprintCallable, Category = "Info")
	bool IsDead();
};
