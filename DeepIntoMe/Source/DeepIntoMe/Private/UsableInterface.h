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

	virtual void OnUsed(ACharacter* User) = 0;

	virtual FString GetActionMessage() = 0;


};
