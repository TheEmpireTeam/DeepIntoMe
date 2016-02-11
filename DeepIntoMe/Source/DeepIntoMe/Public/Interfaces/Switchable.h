// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface.h"
#include "Switchable.generated.h"

UINTERFACE(MinimalAPI)
class USwitchable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ISwitchable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Switch();

	UFUNCTION(BlueprintImplementableEvent)
	bool CanSwitch();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginOverlap();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndOverlap();
};