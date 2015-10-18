// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "Projectile.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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

	//OnActorHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (GEngine != NULL)
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, OtherComp->GetName());
		FPointDamageEvent PointDmg;
		PointDmg.ShotDirection = GetActorRotation().Vector();
		OtherActor->TakeDamage(Damage, PointDmg, Instigator->Controller, this);
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
