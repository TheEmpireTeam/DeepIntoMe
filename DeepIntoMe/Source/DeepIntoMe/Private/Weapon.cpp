// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"
#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	bReplicates = true;

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

	CartridgesLeftInClip = ClipCapacity;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static float Time = 0;
	Time += DeltaTime;

	if (bFiring && Time > (1 / FireRate))
	{
		if (CartridgesLeftInClip > 0)
		{
			Fire();

			// Play fire sound
			USoundBase * ShotSound = GetRandomShotSound();
			if (ShotSound != NULL)
				UGameplayStatics::PlaySoundAtLocation(this, ShotSound, GetActorLocation());

			if (Role == ROLE_Authority)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("DAMAGE: ") + FString::FromInt(CartridgesLeftInClip));
				CartridgesLeftInClip--;
			}
		
			Time = 0;
		}
	}
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
	FVector Location; 
	FRotator Rotation;
	ParentCharacter->GetActorEyesViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MAX_AIM_DISTANCE;

	FCollisionQueryParams Params;
	FVector HitLocation;
	CurrentShotsCount++;

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
	if (Clips > 0)
	{
		CartridgesLeftInClip = ClipCapacity;
		Clips--;
	}
}

bool AWeapon::GetSimulatePhysics()
{
	return bSimulatePhysics;
}

void AWeapon::SetFiringStatus(bool Firing)
{
	if (Role < ROLE_Authority)
		ServerSetFiringStatus(Firing);
	else
	{
		bFiring = Firing;
		if (bFiring)
			CurrentShotsCount = 0;
	}
}

void AWeapon::ServerSetFiringStatus_Implementation(bool Firing)
{
	SetFiringStatus(Firing);
}

bool AWeapon::ServerSetFiringStatus_Validate(bool Firing)
{
	return true;
}

bool AWeapon::GetFiringStatus()
{
	return bFiring;
}

int32 AWeapon::GetCartridgesInClipCount()
{
	return CartridgesLeftInClip;
}

bool AWeapon::IsClipFull()
{
	return (CartridgesLeftInClip == ClipCapacity);
}

int32 AWeapon::GetClipCount()
{
	return Clips;
}

int32 AWeapon::GetClipSize()
{
	return ClipCapacity;
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

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, CartridgesLeftInClip);
	DOREPLIFETIME(AWeapon, CurrentShotsCount);
	DOREPLIFETIME(AWeapon, Clips);
	DOREPLIFETIME(AWeapon, bFiring);
}