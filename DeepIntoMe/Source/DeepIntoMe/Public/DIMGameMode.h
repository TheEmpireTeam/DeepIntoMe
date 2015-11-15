// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "DIMGameMode.generated.h"

UENUM(BlueprintType)
enum class EMultiplayerTeam : uint8 {
	Earth,
	Bailosh,
	Aliens
};

UCLASS()
class DEEPINTOME_API ADIMGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	virtual void RestartPlayer(class AController* NewPlayer) override;
	
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal = TEXT(""));
};
