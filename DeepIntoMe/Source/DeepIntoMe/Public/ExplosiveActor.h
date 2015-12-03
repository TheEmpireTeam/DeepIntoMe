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

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* ExplosionPCS;
	
	UPROPERTY(VisibleDefaultsOnly)
	UAudioComponent* AudioComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Effects")
	UParticleSystem* ExplosionFX;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Effects")
	USoundCue* ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings")
	float ExplosionDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings")
	float ExplosionRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings")
	float Health;
	
	bool bExploded;
	
public:	
	AExplosiveActor(const FObjectInitializer& ObjectInitializer);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void Explode(class AController* EventInstigator);

	UFUNCTION(Reliable, NetMulticast)
	void SimulateExplosion();
};
