// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"
#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Up Collision"));
	PickUpCollision->AttachTo(Mesh);
	//PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnPickUpBeginOverlap);
	//PickUpCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnPickUpEndOverlap);
	FireRate = 1;
	bAutoDestroy = true;

	//----Values only for debug---------
	MaxOffset = 0.01;
	OffsetRate = 0.001;
	//----------------------------------

	bCanBeDamaged = false;
	SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AmmoLeftInClip = ClipCapacity;
	TimeBetweenShots = 1 / FireRate;
	
	if (MinDamage < 0 || MaxDamage < 0 || MaxDamage < MinDamage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Invalid weapon damage!"));
	}
}

void AWeapon::PlayShootSound()
{
	USoundBase * ShotSound = GetRandomShotSound();
	if (ShotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShotSound, GetActorLocation());
	}
}

USoundBase * AWeapon::GetRandomShotSound()
{
	int32 count = ShotSounds.Num();
	return (count == 0) ? NULL : ShotSounds[FMath::RandHelper(count)];
}

void AWeapon::OnUsed(ACharacter* User)
{
	AMainCharacter* MainUser = Cast<AMainCharacter>(User);
	if (MainUser)
	{
		StopDestroyTimer();
		MainUser->AddWeapon(this);
	}
}

FString AWeapon::GetActionMessage()
{
	return ActionMessage;
}

void AWeapon::HandleFiring()
{
	// Decline fire attempt, if clip is empty
	if (AmmoLeftInClip > 0)
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
		{
			HitLocation = OutHit.Location;
		}
		else
		{
			HitLocation = End;
		}

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector FireLocation = Mesh->GetSocketLocation(FireSocketName);
		FVector Direction = (HitLocation - FireLocation).GetClampedToMaxSize(1) + FMath::VRand() * Offset;

		if (Role == ROLE_Authority)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Spawn projectile on server"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Spawn projectile on client"));
		}
		
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileType, FireLocation, Direction.Rotation(), SpawnParameters);
		Projectile->SetInstigator(ParentCharacter);
		
		float DynamicDamage = FMath::FRandRange(MaxDamage, MinDamage);
		Projectile->SetDamage(DynamicDamage);
		
		CurrentShotsCount++;
		AmmoLeftInClip--;
		
		PlayShootSound();
		
		// Update last fire time
		LastFireTime = GetWorld()->GetTimeSeconds();
		
		// Trigger next shot if weapon is automatic
		if (bIsAutomatic && AmmoLeftInClip > 0)
		{
			GetWorldTimerManager().SetTimer(FiringHandleTimer, this, &AWeapon::HandleFiring, TimeBetweenShots, false);
		}
		/*else if (bIsAutomatic && AmmoLeftInClip == 0)
		{
			if (ParentCharacter)
			{
				ParentCharacter->Reload();
			}
		}*/
	}
}

void AWeapon::SetSimulatePhysics(bool SimulatePhysics)
{
	bSimulatePhysics = SimulatePhysics;
	
	if (bSimulatePhysics)
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionProfileName(TEXT("Weapon"));
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

void AWeapon::StartFire()
{	
	if (AmmoLeftInClip > 0)
	{
		bFiring = true;
		CurrentShotsCount = 0;
		
		const float GameTime = GetWorld()->GetTimeSeconds();
		if (GameTime > LastFireTime + TimeBetweenShots)
		{
			// Fire immediately
			HandleFiring();
		}
		else
		{
			// Fire after fire rate based delay
			GetWorldTimerManager().SetTimer(FiringHandleTimer, this, &AWeapon::HandleFiring, LastFireTime + TimeBetweenShots - GameTime, false);
		}
	}
}
	
void AWeapon::StopFire()
{
	bFiring = false;
	
	GetWorldTimerManager().ClearTimer(FiringHandleTimer);
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

void AWeapon::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(AutoDestroyTimer, this, &AWeapon::AutoDestroy, 30.0f, false);
}
	
void AWeapon::StopDestroyTimer()
{
	GetWorldTimerManager().ClearTimer(AutoDestroyTimer);
}

void AWeapon::AutoDestroy()
{
	Destroy();
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