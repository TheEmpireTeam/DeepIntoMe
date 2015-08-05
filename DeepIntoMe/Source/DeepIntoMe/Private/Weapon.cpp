// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "MainCharacter.h"
#include "Weapon.h"




// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Up Collision"));
	PickUpCollision->AttachTo(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	PickUpCollision->bGenerateOverlapEvents = true;
	PickUpCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnPickUpBeginOverlap);
	PickUpCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnPickUpEndOverlap);

	bCanBePicked = false;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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

}

void AWeapon::OnPickUpBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AWeapon::OnPickUpEndOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}