// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "Props/SwitchableLamp.h"


/*
WARNING!
This class is not synchronized yet and can't be applied to multiplayer game
*/

// Sets default values
ASwitchableLamp::ASwitchableLamp(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;

	LightComponent = ObjectInitializer.CreateDefaultSubobject<USpotLightComponent>(this, TEXT("Light"));
	LightComponent->AttachParent = RootComponent;

	SparksPCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Sparks"));
	SparksPCS->bAutoActivate = false;
	SparksPCS->bAutoDestroy = false;
	SparksPCS->AttachParent = RootComponent;

	if (SparksFX)
	{
		SparksPCS->SetTemplate(SparksFX);
	}

	// Lamp is very fragile
	Health = 1;
	bDestroyed = false;
}

void ASwitchableLamp::BeginPlay()
{
	Super::BeginPlay();

	if (bTurnedOn)
	{
		TurnOn();
	}
	else
	{
		TurnOff();
	}
}

void ASwitchableLamp::Switch()
{
	bTurnedOn = !bTurnedOn;

	if (CanSwitch())
	{
		SwitchInternal();
	}
	else
	{
		if (bTurnedOn)
		{
			SparksPCS->ActivateSystem();
		}
		else
		{
			SparksPCS->DeactivateSystem();
		}
	}
}

bool ASwitchableLamp::CanSwitch()
{
	return !bDestroyed;
}

bool ASwitchableLamp::VoltageActive()
{
	return bTurnedOn;
}

float ASwitchableLamp::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (!bDestroyed && Health > 0.0f)
	{
		Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		Health -= Damage;

		if (Health <= 0.0f)
		{
			DestroyObject();
		}

		return Damage;
	}

	return 0.0f;
}

void ASwitchableLamp::DestroyObject()
{
	bDestroyed = true;
	bool bSparks = bTurnedOn;

	TurnOff();
	if (bSparks)
	{
		Switch();
	}
}
