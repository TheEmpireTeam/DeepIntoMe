// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DeepIntoMeHUD.h"

ADeepIntoMeHUD::ADeepIntoMeHUD(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void ADeepIntoMeHUD::OnStateChanged_Implementation(EHUDState NewState)
{
	CurrentState = NewState;
}

EHUDState ADeepIntoMeHUD::GetCurrentState()
{
	return CurrentState;
}
