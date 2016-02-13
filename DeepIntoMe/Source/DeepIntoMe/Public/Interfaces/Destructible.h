// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface.h"
#include "Destructible.generated.h"

UINTERFACE(MinimalAPI)
class UDestructible : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IDestructible
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void DestroyObject() = 0;
};
