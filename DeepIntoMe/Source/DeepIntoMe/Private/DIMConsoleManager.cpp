// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMConsoleManager.h"
#include "DIMPlayerState.h"
#include "MainCharacter.h"


UDIMConsoleManager::UDIMConsoleManager()
{
	OwningController = NULL;

	InitCommandsList();
}
	
void UDIMConsoleManager::InitCommandsList()
{
	CommandsList.Add(TEXT("suicide"));
	FCommandExecutor SuicideExecutor = &UDIMConsoleManager::SuicideCommand;
	CommandExecutors.Add(SuicideExecutor);
	
	CommandsList.Add(TEXT("name"));
	FCommandExecutor ChangeNameExecutor = &UDIMConsoleManager::ChangeNameCommand;
	CommandExecutors.Add(ChangeNameExecutor);
}

void UDIMConsoleManager::SetOwner(ADeepIntoMePlayerController* Controller)
{
	OwningController = Controller;
}

bool UDIMConsoleManager::IsValidConsoleCommand(FString Command)
{
	Command = Command.Trim();
	
	if (!Command.IsEmpty() && Command[0] == '$')
	{
		return true;
	}

	return false;
}

void UDIMConsoleManager::ExecuteCommand(FString Command)
{
	// Trim and remove first '$' character
	Command = Command.Trim();
	Command = Command.Mid(1, Command.Len() - 1);
	
	TArray<FString> CommandParts;
	Command.ParseIntoArray(CommandParts, L" ", true);
	
	if (CommandParts.Num() > 0)
	{
		FString CommandName = CommandParts[0];
		
		int32 CommandIndex = CommandsList.Find(CommandName);
		if (CommandIndex != INDEX_NONE && CommandIndex < CommandExecutors.Num())
		{
			(this->* (CommandExecutors[CommandIndex]))(CommandParts);
		}
	}
}

void UDIMConsoleManager::SuicideCommand(TArray<FString>& CommandParams)
{
	if (OwningController)
	{
		AMainCharacter* Pawn = Cast<AMainCharacter>(OwningController->GetPawn());
		if (Pawn)
		{
			FPointDamageEvent SuicideDamage;
			Pawn->TakeDamage(Pawn->GetHealth() + 1.0f, SuicideDamage, OwningController, Pawn);
		}
	}
}

void UDIMConsoleManager::ChangeNameCommand(TArray<FString>& CommandParams)
{
	if (CommandParams.Num() > 1 && !CommandParams[1].IsEmpty())
	{
		if (OwningController)
		{
			ADIMPlayerState* PlayerState = Cast<ADIMPlayerState>(OwningController->PlayerState);
			if (PlayerState)
			{
				PlayerState->OverrideName(CommandParams[1]);
			}
		}
	}
}

