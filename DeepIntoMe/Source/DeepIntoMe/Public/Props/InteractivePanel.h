// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ExplosiveActor.generated.h"

UCLASS()
class DEEPINTOME_API AExplosiveActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY()
	AActor* ConnectedActor;
	
public:
	void SetConnectedActor(AActor* NewConnectedActor);
	
	AActor* GetConnectedActor();
	
	void DisconnectFromActor();
	
public:
	// ISwitcher interface implementation
	virtual void Switch() override;
};
