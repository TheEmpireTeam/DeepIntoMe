// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "Props/InteractivePanel.h"
#include "Interfaces/Switchable.h"


// Sets default values
AInteractivePanel::AInteractivePanel(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;
}

void AInteractivePanel::SetConnectedActor(AActor* NewConnectedActor)
{
	ConnectedActor = NewConnectedActor;
}

void AInteractivePanel::DisconnectActor()
{
	ConnectedActor = NULL;
}

AActor* AInteractivePanel::GetConnectedActor()
{
	return ConnectedActor;
}

void AInteractivePanel::Switch()
{
	ISwitchable* SwitchableActor = Cast<ISwitchable>(ConnectedActor);
	if (SwitchableActor)
	{
		SwitchableActor->Switch();
	}
}
