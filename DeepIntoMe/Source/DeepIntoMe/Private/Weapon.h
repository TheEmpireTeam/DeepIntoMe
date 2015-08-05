// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UsableInterface.h"
#include "Projectile.h"
#include "Weapon.generated.h"

//Warning: GOVNOKODIK 
//-------------------------------------
//Forward declaration of CPPMainCharacter
//to use AddWeapon() method

class AMainCharacter;

UCLASS()
class AWeapon : public AActor, public IUsableInterface
{
	GENERATED_BODY()

private:

	AMainCharacter* ParentCharacter;

	//Message that will appear on the screen
	UPROPERTY(EditAnywhere, Category = ActionSettings)
	FString ActionMessage;

	UPROPERTY(EditAnywhere, Category = Firing)
	TSubclassOf<AProjectile> ProjectileType;

	//Name of a socket for Firing
	UPROPERTY(EditAnywhere, Category = Firing)
	FName FireSocketName;

	//Weapon's collision for pick up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUpCollision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PickUpCollision;

	//Weapon's mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	bool bSimulatePhysics;

	bool bFiring;

public:

	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Fire();

	void SetSimulatePhysics(bool SimulatePhyics);

	bool GetSimulatePhysics();

	void SetParentCharacter(AMainCharacter* NewParentCharacter);

	void SetFiringStatus(bool Firing);

	bool GetFiringStatus();

	UFUNCTION()
	void OnPickUpBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnPickUpEndOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	//IUsableInterface overrided methods
	//-------------------------------------
	virtual void OnUsed(ACharacter* User) override;

	virtual FString GetActionMessage() override;
	//-------------------------------------
	
};
