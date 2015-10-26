// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "DIMGameInstance.generated.h"

UCLASS()
class UDIMGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	FString Nickname;

public:
	UFUNCTION(BlueprintCallable, Category = "Player Info")
	void SetNickname(FString NewNickname);

	UFUNCTION(BlueprintCallable, Category = "Player Info")
	FString GetNickname();
	
};
