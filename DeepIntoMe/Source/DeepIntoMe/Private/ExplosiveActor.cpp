// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "ExplosiveActor.h"


// Sets default values
AExplosiveActor::AExplosiveActor(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;

	ExplosionPCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Explosion"));
	ExplosionPCS->bAutoActivate = false;
	ExplosionPCS->bAutoDestroy = false;
	ExplosionPCS->AttachParent = RootComponent;

	AudioComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->bAutoDestroy = false;
	AudioComp->AttachParent = RootComponent;

	// Let the bomb be thrown and roll around
	MeshComp->SetSimulatePhysics(true);

	SetReplicates(true);
	bReplicateMovement = true;

	bExploded = false;
}

void AExplosiveActor::Explode(class AController* EventInstigator)
{
	// Runs on all clients (NetMulticast)
	SimulateExplosion();

	// Apply damage to player, enemies and environmental objects
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		this, ExplosionDamage, 15.0f,
		GetActorLocation(), ExplosionRadius / 4.0f, ExplosionRadius, 0.1f,
		DamageType, IgnoreActors, this, EventInstigator
	);

	/* Allow clients to show FX before deleting */
	SetLifeSpan(3.0f);
}

void AExplosiveActor::SimulateExplosion_Implementation()
{
	// First deactivate all running fuze effects
	AudioComp->Stop();

	MeshComp->SetVisibility(false, false);

	// Activate all explosion effects
	if (ExplosionSound)
	{
		AudioComp->SetSound(ExplosionSound);
		AudioComp->Play();
	}
	if (ExplosionFX)
	{
		ExplosionPCS->SetTemplate(ExplosionFX);
		ExplosionPCS->ActivateSystem();
	}
}

float AExplosiveActor::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (!bExploded && Health > 0)
	{
		Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		Health -= Damage;
		
		if (Health <= 0)
		{
			bExploded = true;
			Explode(EventInstigator);
		}
	}

	return Damage;
}
