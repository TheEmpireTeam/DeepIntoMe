#include "DeepIntoMe.h"
#include "Props/InteractivePanel.h"
#include "Interfaces/Switchable.h"


AInteractivePanel::AInteractivePanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;
}

void AInteractivePanel::Switch()
{
	ISwitchable* SwitchableActor = Cast<ISwitchable>(ConnectedActor);
	if (SwitchableActor)
	{
		SwitchableActor->Switch();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected Actor is of non-switchable type"));
	}
}

void AInteractivePanel::SetConnectedActor(AActor* NewConnectedActor)
{
	ConnectedActor = NewConnectedActor;
}
	
AActor* AInteractivePanel::GetConnectedActor()
{
	return ConnectedActor;
}
	
void AInteractivePanel::DisconnectFromActor()
{
	ConnectedActor = NULL;
}
