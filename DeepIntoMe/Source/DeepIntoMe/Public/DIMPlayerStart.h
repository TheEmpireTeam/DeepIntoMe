// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "DIMGameMode.h"
#include "DIMPlayerStart.generated.h"

UCLASS()
class DEEPINTOME_API ADIMPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Spawn Properties")
	EMultiplayerTeam OwnerTeam;
};
