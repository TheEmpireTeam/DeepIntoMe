// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeHUD.h"
#include "DeepIntoMePlayerController.h"

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
	Health = 100.0f;
	
	OnActorBeginOverlap.AddDynamic(this, &AMainCharacter::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMainCharacter::OnEndOverlap);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bTestIsDead = false;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponType, FVector(0, 0, 1000), FRotator(0, 0, 0), SpawnParameters);

	if (NewWeapon)
	{
		AddWeapon(NewWeapon);
	}

	StopRunning();
}

bool AMainCharacter::IsMagazineEmpty()
{
	return (Weapon) ? (Weapon->GetAmmoInClipCount() == 0) : true;
}

bool AMainCharacter::CanReload()
{
	return (Weapon) ? Weapon->CanReload() : false;
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
	InputComponent->BindAction("Drop Item", IE_Pressed, this, &AMainCharacter::NetMulticastDropWeapon);
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
	if (Weapon && !bReloading && Weapon->GetAmmoInClipCount() > 0)
	{
		bFiring = true;
		Weapon->StartFire();
	}
	
	if (Role < ROLE_Authority)
	{
		ServerStartFire();
	}
}

void AMainCharacter::ServerStartFire_Implementation()
{
	StartFire();
}

bool AMainCharacter::ServerStartFire_Validate()
{
	return true;
}

void AMainCharacter::StopFire()
{
	if (Weapon)
	{
		bFiring = false;
		Weapon->StopFire();
	}
	
	if (Role < ROLE_Authority)
	{
		ServerStopFire();
	}
}

void AMainCharacter::ServerStopFire_Implementation()
{
	StopFire();
}

bool AMainCharacter::ServerStopFire_Validate()
{
	return true;
}


void AMainCharacter::Reload()
{
	if (Weapon && CanReload())
	{
		bReloading = true;
		Weapon->Reload();
	}
	
	if (Role < ROLE_Authority)
	{
		ServerReload();
	}
}

void AMainCharacter::ServerReload_Implementation()
{
	Reload();
}

bool AMainCharacter::ServerReload_Validate()
{
	return true;
}

void AMainCharacter::StartAiming()
{
	SetAiming(true);
}

void AMainCharacter::StopAiming()
{
	SetAiming(false);
}

void AMainCharacter::SetAiming(bool Aiming)
{
	bAiming = Aiming;

	if (Role < ROLE_Authority)
	{
		ServerSetAiming(Aiming);
	}
}

void AMainCharacter::ServerSetAiming_Implementation(bool Aiming)
{
	SetAiming(Aiming);
}

bool AMainCharacter::ServerSetAiming_Validate(bool Aiming)
{
	return true;
}

void AMainCharacter::StartCrouching()
{
	SetChourching(true);
}

void AMainCharacter::StopCrouching()
{
	SetChourching(false);
}

void AMainCharacter::SetChourching(bool Crouching)
{
	bCrouching = Crouching;
	
	if (Role < ROLE_Authority)
	{
		ServerSetChourching(Crouching);
	}
}
	
void AMainCharacter::ServerSetChourching_Implementation(bool Crouching)
{
	SetChourching(Crouching);
}

bool AMainCharacter::ServerSetChourching_Validate(bool Crouching)
{
	return true;
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
		
	if (Role < ROLE_Authority)
	{
		ServerSetRunningStatus(Running);
	}
}

void AMainCharacter::ServerSetRunningStatus_Implementation(bool Running)
{
	SetRunningStatus(Running);
}

bool AMainCharacter::ServerSetRunningStatus_Validate(bool Running)
{
	return true;
}

void AMainCharacter::AddWeapon(AWeapon* NewWeapon)
{
	if (Weapon)
	{
		NetMulticastDropWeapon();
	}
	
	if (NewWeapon)
	{
		AttachWeaponToCharacter(NewWeapon);
	}
}

USkeletalMeshComponent* AMainCharacter::GetWeaponMesh()
{
	return (Weapon) ? Weapon->GetWeaponMesh() : NULL;
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

	if (UsableItem)
	{
		if (GEngine)
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

	if (UsableItem)
	{
		Items.Remove(OtherActor->GetHumanReadableName());
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	CheckDeath(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return DamageAmount;
}

void AMainCharacter::CheckDeath(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Role < ROLE_Authority)
	{
		ServerCheckDeath(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		if (!bTestIsDead)
		{
			Health -= DamageAmount;

			if (Health < 0)
			{	
				if (EventInstigator)
				{
					if (EventInstigator->GetPawn() != this)
					{
						GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, EventInstigator->GetPawn()->PlayerState->PlayerName);
						GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("WAS KILLED BY"));
						GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, PlayerState->PlayerName);
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, PlayerState->PlayerName);
						GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("HAS KILLED HIMSELF"));
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, PlayerState->PlayerName);
					GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("HAS BEEN KILLED BY UNDETECTED REASON"));
				}
				
				OnDying();
			}
		}
	}
}

void AMainCharacter::ServerCheckDeath_Implementation(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	CheckDeath(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool AMainCharacter::ServerCheckDeath_Validate(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}

void AMainCharacter::ServerDeath_Implementation()
{
	OnDying();
}

bool AMainCharacter::ServerDeath_Validate()
{
	return true;
}

void AMainCharacter::OnDying()
{
	Health = 0;
	bTestIsDead = true;

	NetMulticastDropWeapon();
	
	SetSpectatorMode();
}

void AMainCharacter::SetSpectatorMode()
{	
	ADeepIntoMePlayerController* PlayerController = Cast<ADeepIntoMePlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->SetSpectatorMode();
	}
			
	Destroy();
}

void AMainCharacter::AttachWeaponToCharacter(AWeapon* NewWeapon)
{
	Weapon = NewWeapon;
	Weapon->SetParentCharacter(this);
	Weapon->SetSimulatePhysics(false);
	Weapon->AttachRootComponentTo(FirstPersonMesh, SocketName, EAttachLocation::SnapToTarget);
}

void AMainCharacter::DropWeapon()
{
	if (Weapon)
	{
		Weapon->StopFire();
		Weapon->DetachRootComponentFromParent(true);
		Weapon->SetParentCharacter(NULL);	
		Weapon->SetSimulatePhysics(true);
		Weapon = NULL;
	}
}

void AMainCharacter::NetMulticastDropWeapon_Implementation()
{
	DropWeapon();
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, Health);
	DOREPLIFETIME(AMainCharacter, bTestIsDead);
	
	// Firing & movement properties
	DOREPLIFETIME(AMainCharacter, bReloading);
	DOREPLIFETIME(AMainCharacter, bAiming);
	DOREPLIFETIME(AMainCharacter, bCrouching);
	DOREPLIFETIME(AMainCharacter, bRunning);
}