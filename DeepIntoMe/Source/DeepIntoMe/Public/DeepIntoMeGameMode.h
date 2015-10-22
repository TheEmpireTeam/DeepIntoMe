// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "DeepIntoMeGameMode.generated.h"


USTRUCT()
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	FString Nickname;
};

UCLASS()
class DEEPINTOME_API ADeepIntoMeGameMode : public AGameMode
{
	GENERATED_BODY()

	ADeepIntoMeGameMode(const FObjectInitializer& ObjectInitializer);
	
/*public:
	UPROPERTY(Replicated)
	TArray<FPlayerInfo> ConnectedPlayers;*/

};
