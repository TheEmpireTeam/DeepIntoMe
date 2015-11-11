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

	//----Values only for debug---------
	MaxOffset = 0.01;
	OffsetRate = 0.001;
	//----------------------------------

	SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AmmoLeftInClip = ClipCapacity;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	static float Time = 0;
	Time += DeltaTime;
	
	if (bFiring && Time > (1 / FireRate))
	{
		if (AmmoLeftInClip > 0)
		{
			Fire();
		
			Time = 0;
		}
	}
}

void AWeapon::PlayShootSound()
{
	USoundBase * ShotSound = GetRandomShotSound();
	if (ShotSound != NULL)
		UGameplayStatics::PlaySoundAtLocation(this, ShotSound, GetActorLocation());
}

USoundBase * AWeapon::GetRandomShotSound()
{
	int32 count = ShotSounds.Num();
	if (count == 0)
		return NULL;
	else
	{
		int32 randomIndex = FMath::RandRange(0, count - 1);
		return ShotSounds[randomIndex];
	}
}

void AWeapon::OnUsed(ACharacter* User)
{
	AMainCharacter* MainUser = Cast<AMainCharacter>(User);
	if (MainUser)
		MainUser->AddWeapon(this);
}

FString AWeapon::GetActionMessage()
{
	return ActionMessage;
}

void AWeapon::Fire()
{
	FVector Location; 
	FRotator Rotation;
	ParentCharacter->GetActorEyesViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MAX_AIM_DISTANCE;

	FCollisionQueryParams Params;
	FVector HitLocation;	

	FHitResult OutHit;
	float Offset = FMath::Clamp<float>(CurrentShotsCount * OffsetRate, 0, MaxOffset);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Location, End, ECollisionChannel::ECC_Visibility, Params))
		HitLocation = OutHit.Location;
	else
		HitLocation = End;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector FireLocation = Mesh->GetSocketLocation(FireSocketName);
	FVector Direction = (HitLocation - FireLocation).GetClampedToMaxSize(1) + FMath::VRand() * Offset;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileType, FireLocation, Direction.Rotation(), SpawnParameters);
	Projectile->SetInstigator(ParentCharacter);
	Projectile->SetDamage(Damage);
	
	CurrentShotsCount++;
	AmmoLeftInClip--;
	
	PlayShootSound();
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
	if (AmmoLeftTotal > 0)
	{
		// How many ammo we need to fill current clip to full size
		int32 AmmoNeedToReload = FMath::Min(ClipCapacity - AmmoLeftInClip, AmmoLeftTotal);
		
		if (AmmoNeedToReload > 0)
		{
			AmmoLeftInClip += AmmoNeedToReload;
			AmmoLeftTotal -= AmmoNeedToReload;
		}
	}
}

bool AWeapon::GetSimulatePhysics()
{
	return bSimulatePhysics;
}

void AWeapon::SetFiringStatus(bool Firing)
{
	bFiring = Firing;
	if (bFiring)
		CurrentShotsCount = 0;
}

bool AWeapon::GetFiringStatus()
{
	return bFiring;
}

int32 AWeapon::GetAmmoInClipCount()
{
	return AmmoLeftInClip;
}

bool AWeapon::IsClipFull()
{
	return (AmmoLeftInClip == ClipCapacity);
}

int32 AWeapon::GetRemainingAmmoCount()
{
	return AmmoLeftTotal;
}

int32 AWeapon::GetClipSize()
{
	return ClipCapacity;
}

bool AWeapon::CanReload()
{
	return (!IsClipFull() && AmmoLeftTotal > 0);
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

FString AWeapon::GetUsableName()
{
	return InventoryName;
}