// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "Projectile.h"
#include "MainCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = Collision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3020.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	Damage = 0;
}

void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Role < ROLE_Authority)
	//{
	ServerOnHit(OtherActor, OtherComp, NormalImpulse, Hit);
	//}
}

void AProjectile::ServerOnHit_Implementation(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (Hit.GetActor() != NULL)
		{
			if (GEngine != NULL)
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, OtherComp->GetName());

			FPointDamageEvent PointDmg;
			PointDmg.ShotDirection = GetActorRotation().Vector();

			AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
			if (Character)
				Character->ManualTakeDamage(Damage);

			/*if (Instigator)
			{
				OtherActor->TakeDamage(Damage, PointDmg, Instigator->Controller, this);
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("NOT NULL"));
			}	
			else
			{
				OtherActor->TakeDamage(Damage, PointDmg, NULL, this);
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("NULL"));
			}*/
		}

		Destroy();
	}
}

bool AProjectile::ServerOnHit_Validate(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return true;
}

void AProjectile::SetInstigator(ACharacter* NewInstigator)
{
	Instigator = NewInstigator;
}

void AProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}
