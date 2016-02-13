// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/Switchable.h"
#include "Interfaces/Destructible.h"
#include "SwitchableLamp.generated.h"

UCLASS()
class DEEPINTOME_API ASwitchableLamp : public AActor, public ISwitchable, public IDestructible
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Light")
	USpotLightComponent* LightComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* SparksPCS;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* SparksFX;
	
public:
	ASwitchableLamp(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void SwitchInternal();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void TurnOn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void TurnOff();

	bool VoltageActive();
	
public:
	// ISwitchable interface implementation
	virtual void Switch() override;

	virtual bool CanSwitch() override;

	// IDestructible interface implementation
	virtual void DestroyObject() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bTurnedOn;

private:
	float Health;

	bool bDestroyed;
};
