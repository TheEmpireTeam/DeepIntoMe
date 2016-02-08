#pragma once

#include "Interface.h"
#include "GameFramework/Character.h"
#include "ISwitchable.generated.h"

UINTERFACE(MinimalAPI)
class USwitchable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ISwitchable
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void Switch() = NULL;

	virtual bool CanSwitch() = NULL;
};