// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "DeepIntoMePlayerController.h"
#include "DIMConsoleManager.generated.h"


DECLARE_DELEGATE_OneParam(FCommandExecutor, TArray<FString>&);


UCLASS(Blueprintable)
class DEEPINTOME_API UDIMConsoleManager : public UObject
{
	GENERATED_BODY()
	
private:
	typedef void (UDIMConsoleManager::*FCommandExecutor)(TArray<FString>& CommandParams);
 
	TArray<FString> CommandsList;
	
	TArray<FCommandExecutor> CommandExecutors;
	
	ADeepIntoMePlayerController* OwningController;
	
	
public:
	UDIMConsoleManager();
	
	void InitCommandsList();
	
	UFUNCTION(BlueprintCallable, Category = "Console System")
	void SetOwner(ADeepIntoMePlayerController* Controller);
	
	/*UFUNCTION(BlueprintImplementableEvent, Category = "Console System")
	void InitAdditionalCommandsList();*/

	UFUNCTION(BlueprintCallable, Category = "Console System")
	bool IsValidConsoleCommand(FString Command);
	
	UFUNCTION(BlueprintCallable, Category = "Console System")
	void ExecuteCommand(FString Command);
	
	
	// Command executors
	void SuicideCommand(TArray<FString>& CommandParams);
	void ChangeNameCommand(TArray<FString>& CommandParams);
	void StatsCommand(TArray<FString>& CommandParams);
	void RespawnCommand(TArray<FString>& CommandParams);
	
};
