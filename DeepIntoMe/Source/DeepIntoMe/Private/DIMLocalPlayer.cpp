// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMLocalPlayer.h"


FString UDIMLocalPlayer::GetNickname() const
{
	/* Try to fetch a nickname from the online subsystem (eg. Steam) if available */
	FString NickName = Super::GetNickname();

	// Fall back if no nickname was available through the online subsystem.
	if (NickName.IsEmpty())
	{
		const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
		NickName = FPlatformProcess::ComputerName() + Suffix;
		NickName = TEXT("Test Nick");
	}

	return NickName;
}
