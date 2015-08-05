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
		if (GEngine != NULL)
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString("Got Spawn Weapon"));
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

	InputComponent->BindAction("Experiment", IE_Pressed, this, &AMainCharacter::Experiment);

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

//!!!!!Experimental!!!!
void AMainCharacter::AddWeapon(AWeapon* NewWeapon)
{
	if (Weapon != NULL)
	{
		Weapon->DetachRootComponentFromParent(true);
		//Weapon->Mesh->DetachFromParent(true);
		Weapon->Mesh->SetSimulatePhysics(true);
		Weapon->Mesh->SetCollisionProfileName(TEXT("BlockAll"));
		Weapon->PickUpCollision->bGenerateOverlapEvents = true;
		Weapon->PickUpCollision->SetCollisionProfileName(TEXT("OverlapAll"));
		Weapon = NULL;
	}
	Weapon = NewWeapon;
	Weapon->Mesh->SetSimulatePhysics(false);
	Weapon->Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Weapon->PickUpCollision->bGenerateOverlapEvents = false;
	Weapon->PickUpCollision->SetCollisionProfileName(TEXT("NoCollison"));
	//Weapon->Mesh->AttachTo(FirstPersonMesh, SocketName);
	Weapon->AttachRootComponentTo(FirstPersonMesh, SocketName, EAttachLocation::SnapToTarget);
}

void AMainCharacter::Experiment()
{
	if (UsableActor != NULL)
	{
		UsableActor->OnUsed(this);
	}
}

void AMainCharacter::OnBeginOverlap(AActor* OtherActor)
{
	UsableActor = Cast<IUsableInterface>(OtherActor);

	if (GEngine != NULL && UsableActor != NULL)
	{
		FString Message = UsableActor->GetActionMessage();
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, Message);
	}
}

void AMainCharacter::OnEndOverlap(AActor* OtherActor)
{

}