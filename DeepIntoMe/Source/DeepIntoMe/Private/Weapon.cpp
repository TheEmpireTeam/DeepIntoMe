// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"
#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Up Collision"));
	PickUpCollision->AttachTo(Mesh);
	PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnPickUpBeginOverlap);
	PickUpCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnPickUpEndOverlap);
	FireRate = 1;

	SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	static float Time = 0;
	Time += DeltaTime;

	if (bFiring && Time > (1/FireRate))
	{
		if (CurrentBulletCount)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::FromInt(CurrentBulletCount));
			}			
			Fire();
			CurrentBulletCount--;
			Time = 0;
		}
		else
		{
			Reload();
		}
	}
}

void AWeapon::OnUsed(ACharacter* User)
{
	AMainCharacter* MainUser = Cast<AMainCharacter>(User);
	if (MainUser != NULL)
	{
		MainUser->AddWeapon(this);
	}
}

FString AWeapon::GetActionMessage()
{
	return ActionMessage;
}

void AWeapon::Fire()
{
	/**Warning: Experimental GOVNOKOD*/
	FHitResult OutHit;
	FVector Start; 
	FRotator Rotation;
	ParentCharacter->GetActorEyesViewPoint(Start, Rotation);
	FVector End = Start + Rotation.Vector() * 100000;
	FCollisionQueryParams Params;
	FVector HitLocation;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		HitLocation = OutHit.Location;
	}
	else
	{
		HitLocation = End;
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoCollisionFail = true;
	FVector FireLocation = Mesh->GetSocketLocation(FireSocketName);
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileType, FireLocation, (HitLocation - FireLocation).Rotation(), SpawnParameters);
	Projectile->SetInstigator(ParentCharacter);
}

void AWeapon::SetSimulatePhysics(bool SimulatePhysics)
{
	bSimulatePhysics = SimulatePhysics;
	if (bSimulatePhysics)
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
		PickUpCollision->bGenerateOverlapEvents = true;
		PickUpCollision->SetCollisionProfileName(TEXT("Trigger"));
	}
	else
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionProfileName(TEXT("NoCollision"));
		PickUpCollision->bGenerateOverlapEvents = false;
		PickUpCollision->SetCollisionProfileName(TEXT("NoCollison"));
	}
}

void AWeapon::Reload()
{
	if (Clips)
	{
		CurrentBulletCount = ClipCapacity;
		Clips--;
	}
}

bool AWeapon::GetSimulatePhysics()
{
	return bSimulatePhysics;
}

void AWeapon::SetFiringStatus(bool Firing)
{
	bFiring = Firing;
}

bool AWeapon::GetFiringStatus()
{
	return bFiring;
}

int32 AWeapon::GetCurrentBulletCount()
{
	return CurrentBulletCount;
}

int32 AWeapon::GetCurrentClipCount()
{
	return Clips;
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh()
{
	return Mesh;
}

void AWeapon::SetParentCharacter(AMainCharacter* NewParentCharacter)
{
	ParentCharacter = NewParentCharacter;
}


void AWeapon::OnPickUpBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AWeapon::OnPickUpEndOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}