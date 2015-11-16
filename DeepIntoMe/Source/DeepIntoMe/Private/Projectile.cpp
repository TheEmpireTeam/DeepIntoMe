// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "Projectile.h"
#include "MainCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
	bReplicates = true;
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = Collision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 10020.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	Damage = 0;
}

void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (Hit.GetActor() != NULL)
		{
			//if (GEngine != NULL)
				//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, OtherComp->GetName());

			FPointDamageEvent PointDmg;
			PointDmg.ShotDirection = GetActorRotation().Vector();

			if (Role == ROLE_Authority && Instigator)
				OtherActor->TakeDamage(Damage, PointDmg, Instigator->Controller, this);
		}

		Destroy();
	}
}

void AProjectile::SetInstigator(ACharacter* NewInstigator)
{
	Instigator = NewInstigator;
}

void AProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}