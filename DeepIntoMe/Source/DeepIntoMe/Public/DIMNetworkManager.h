// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DIMNetworkManager.generated.h"

USTRUCT()
struct FPlayerNetworkInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Info")
	int32 NetworkId;

	UPROPERTY(BlueprintReadWrite, Category = "Info")
	FString Nickname;

	UPROPERTY(BlueprintReadWrite, Category = "Info")
	float Health;
};

UCLASS()
class UDIMNetworkManager : public UObject
{
	GENERATED_BODY()

	UDIMNetworkManager();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Game Statistics", Replicated)
	TArray<FPlayerNetworkInfo> PlayersInfo;

public:
	UFUNCTION()
	int32 RegisterPlayer(ADIMPlayerState* PlayerState, FString PlayerName);

	UFUNCTION(BlueprintCallable, Category = "Game Statistics")
	FPlayerNetworkInfo GetPlayerInfo(int32 PlayerIndex);

private:
	UPROPERTY(Replicated)
	int32 LastPlayerIndex;
};
