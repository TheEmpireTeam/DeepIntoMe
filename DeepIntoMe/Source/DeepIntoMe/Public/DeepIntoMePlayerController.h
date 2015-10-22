// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "DeepIntoMePlayerController.generated.h"

UCLASS()
class ADeepIntoMePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADeepIntoMePlayerController();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetPlayerName(FString Name);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	FString GetPlayerName();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;
	
};
