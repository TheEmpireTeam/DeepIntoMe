// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
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
	SpawnParameters.bNoCollisionFail = true;
	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponType, FVector(0, 0, 1000), FRotator(0, 0, 0), SpawnParameters);
	if (NewWeapon != NULL)
	{
		AddWeapon(NewWeapon);
	}
}

// Called every frame
void AMainCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
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
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * BaseRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookRight(float Value)
{
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
	if (Weapon)
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
	if (Weapon)
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

void AMainCharacter::AddWeapon(AWeapon* NewWeapon)
{
	if (Weapon != NULL)
	{
		DetachWeaponFromCharacter(NewWeapon->GetActorLocation());
	}
	AttachWeaponToCharacter(NewWeapon);
}

USkeletalMeshComponent* AMainCharacter::GetWeaponMesh()
{
	return Weapon->GetWeaponMesh();
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
	Health -= DamageAmount;
	if (Health < 0)
	{
		OnDying();
		Destroy();
	}
	return DamageAmount;
}

void AMainCharacter::OnDying()
{
	DetachWeaponFromCharacter(Weapon->GetActorLocation());
}

void AMainCharacter::AttachWeaponToCharacter(AWeapon* NewWeapon)
{
	Weapon = NewWeapon;
	Weapon->SetParentCharacter(this);
	Weapon->SetSimulatePhysics(false);
	Weapon->AttachRootComponentTo(FirstPersonMesh, SocketName, EAttachLocation::SnapToTarget);
}

void AMainCharacter::DetachWeaponFromCharacter(FVector WeaponLocation)
{
	Weapon->DetachRootComponentFromParent(false);
	Weapon->SetParentCharacter(NULL);
	Weapon->SetSimulatePhysics(true);
	Weapon->SetActorLocation(WeaponLocation);
	Weapon = NULL;
}