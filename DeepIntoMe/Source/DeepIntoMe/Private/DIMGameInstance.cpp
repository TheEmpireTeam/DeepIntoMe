// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMGameInstance.h"


void UDIMGameInstance::SetNickname(FString NewNickname)
{
	Nickname = NewNickname;
}

FString UDIMGameInstance::GetNickname()
{
	if (Nickname.IsEmpty())
	{
		const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
		Nickname = FPlatformProcess::ComputerName() + Suffix;
	}

	return Nickname;
}
