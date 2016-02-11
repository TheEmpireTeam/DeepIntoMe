// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface.h"
#include "Switcher.generated.h"

UINTERFACE(MinimalAPI)
class USwitcher : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ISwitcher
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Switch();
};