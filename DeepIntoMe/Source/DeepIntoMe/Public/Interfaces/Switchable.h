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
	virtual void Switch() = 0;

	virtual bool CanSwitch() = 0;
	
	//virtual void OnBeginOverlap() = 0;
	
	//virtual void OnEndOverlap() = 0;
};