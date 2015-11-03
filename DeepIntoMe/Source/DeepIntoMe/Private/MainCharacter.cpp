// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"
#include "DIMPlayerState.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to replicate
	bReplicates = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseRate = 45;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	Camera->AttachTo(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;
	FirstPersonMesh->AttachTo(Camera);
	Health = 100;
	
	OnActorBeginOverlap.AddDynamic(this, &AMainCharacter::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMainCharacter::OnEndOverlap);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponType, FVector(0, 0, 1000), FRotator(0, 0, 0), SpawnParameters);

	if (NewWeapon)
		AddWeapon(NewWeapon);

	StopRunning();
}

bool AMainCharacter::IsMagazineEmpty()
{
	if (Weapon) 
		return (Weapon->GetCartridgesInClipCount() == 0);

	return true;
}

bool AMainCharacter::CanReload()
{
	if (Weapon)
		return (Weapon->GetClipCount() > 0 && !Weapon->IsClipFull());

	return false;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	InputComponent->BindAxis("LookRight", this, &AMainCharacter::LookRight);
	InputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	InputComponent->BindAction("Use", IE_Pressed, this, &AMainCharacter::UseItem);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::StopFire);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::Reload);
	InputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::StartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::StopAiming);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouching);
	InputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouching);
	InputComponent->BindAction("Running", IE_Pressed, this, &AMainCharacter::StartRunning);
	InputComponent->BindAction("Running", IE_Released, this, &AMainCharacter::StopRunning);
}

void AMainCharacter::LookUp(float Value)
{
	YLookRate = Value;
	AddControllerPitchInput(Value * BaseRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookRight(float Value)
{
	XLookRate = Value;
	AddControllerYawInput(Value * BaseRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMainCharacter::StartFire()
{
	if (Weapon && !bReloading && !IsMagazineEmpty())
	{
		bFiring = true;
		Weapon->SetFiringStatus(true);
	}
}

void AMainCharacter::StopFire()
{
	if (Weapon)
	{
		bFiring = false;
		Weapon->SetFiringStatus(false);
	}
}


void AMainCharacter::Reload()
{
	if (Weapon && CanReload())
	{
		bReloading = true;
		Weapon->Reload();
	}
}

void AMainCharacter::StartAiming()
{
	bAiming = true;
}

void AMainCharacter::StopAiming()
{
	bAiming = false;
}

void AMainCharacter::StartCrouching()
{
	bCrouching = true;
}

void AMainCharacter::StopCrouching()
{
	bCrouching = false;
}

void AMainCharacter::StartRunning()
{
	SetRunningStatus(true);
}

void AMainCharacter::StopRunning()
{
	SetRunningStatus(false);
}

void AMainCharacter::SetRunningStatus(bool Running)
{
	bRunning = Running;
	if (bRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
}

void AMainCharacter::AddWeapon(AWeapon* NewWeapon)
{
	if (Weapon != NULL)
	{
		DetachWeaponFromCharacter(NewWeapon->GetTransform());
	}
	AttachWeaponToCharacter(NewWeapon);
}

USkeletalMeshComponent* AMainCharacter::GetWeaponMesh()
{
	if (Weapon)
		return Weapon->GetWeaponMesh();
	else
		return NULL;
}

AWeapon* AMainCharacter::GetWeapon()
{
	return Weapon;
}

float AMainCharacter::GetHealth()
{
	return Health;
}

void AMainCharacter::UseItem()
{
	if (Items.Num())
	{
		auto Iterator = Items.CreateIterator();
		Iterator.Value()->OnUsed(this);
	}
}

void AMainCharacter::OnBeginOverlap(AActor* OtherActor)
{
	IUsableInterface* UsableItem = Cast<IUsableInterface>(OtherActor);

	if (UsableItem != NULL)
	{
		if (GEngine != NULL)
		{
			FString Message = UsableItem->GetActionMessage();
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, Message);
		}
		Items.Add(OtherActor->GetHumanReadableName(), UsableItem);
	}
}

void AMainCharacter::OnEndOverlap(AActor* OtherActor)
{
	IUsableInterface* UsableItem = Cast<IUsableInterface>(OtherActor);

	if (UsableItem != NULL)
	{
		Items.Remove(OtherActor->GetHumanReadableName());
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Role < ROLE_Authority)
	{
		ServerTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		return DamageAmount;
	}
	else
	{
		Health -= DamageAmount;

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("TakeDamage called!"));
		if (Health < 0)
		{
			OnDying();
		}

		return DamageAmount;
	}
}

void AMainCharacter::ServerTakeDamage_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool AMainCharacter::ServerTakeDamage_Validate(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}

void AMainCharacter::ManualTakeDamage(float DamageAmount)
{
	if (Role < ROLE_Authority)
		ServerManualTakeDamage(DamageAmount);
	else
	{
		Health -= DamageAmount;
	}
}

void AMainCharacter::ServerManualTakeDamage_Implementation(float DamageAmount)
{
	ManualTakeDamage(DamageAmount);
}

bool AMainCharacter::ServerManualTakeDamage_Validate(float DamageAmount)
{
	return true;
}

void AMainCharacter::OnDying()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Player died!"));

	//DetachWeaponFromCharacter(Weapon->GetTransform());
}

void AMainCharacter::AttachWeaponToCharacter(AWeapon* NewWeapon)
{
	Weapon = NewWeapon;
	Weapon->SetParentCharacter(this);
	Weapon->SetSimulatePhysics(false);
	Weapon->AttachRootComponentTo(FirstPersonMesh, SocketName, EAttachLocation::SnapToTarget);
}

void AMainCharacter::DetachWeaponFromCharacter(FTransform NewTransform)
{
	Weapon->DetachRootComponentFromParent(false);
	Weapon->SetParentCharacter(NULL);
	Weapon->SetSimulatePhysics(true);
	Weapon->SetActorTransform(NewTransform);
	Weapon = NULL;
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to every client, no special condition required
	DOREPLIFETIME(AMainCharacter, Health);
}
