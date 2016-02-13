// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/Switcher.h"
#include "InteractivePanel.generated.h"

UCLASS()
class DEEPINTOME_API AInteractivePanel : public AActor, public ISwitcher
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Panel Mechanism")
	AActor* ConnectedActor;
	
public:
	AInteractivePanel(const FObjectInitializer& ObjectInitializer);

	void SetConnectedActor(AActor* NewConnectedActor);

	void DisconnectActor();

	AActor* GetConnectedActor();

	
public:
	// ISwitcher interface implementation
	virtual void Switch() override;
};
