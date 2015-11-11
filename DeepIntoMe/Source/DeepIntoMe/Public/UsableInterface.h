// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface.h"
#include "GameFramework/Character.h"
#include "UsableInterface.generated.h"


UINTERFACE(MinimalAPI)
class UUsableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()

};

class IUsableInterface
{
	GENERATED_IINTERFACE_BODY()

	//Event for object to do some shit
	virtual void OnUsed(ACharacter* User) = 0;

	//A message that will be sent to HUD for using some shit
	virtual FString GetActionMessage() = 0;
	
public:
	virtual FString GetUsableName() = 0;
};
