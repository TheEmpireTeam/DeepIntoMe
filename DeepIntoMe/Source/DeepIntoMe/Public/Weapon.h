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

	const int MAX_AIM_DISTANCE = 100000; 

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
	USkeletalMeshComponent* Mesh;

	//Amount of bullets per second
	UPROPERTY(EditAnywhere, Category = Firing)
	float FireRate;

	//Maximum offset for firing 
	UPROPERTY(EditAnywhere, Category = Firing)
	float MaxOffset;

	//offset rate for firing 
	UPROPERTY(EditAnywhere, Category = Firing)
	float OffsetRate;

	//Does weapon simulate physics
	UPROPERTY()
	bool bSimulatePhysics;

	//Is weapon firing right now
	UPROPERTY()
	bool bFiring;

	//Amount of clips
	UPROPERTY(EditAnywhere, Category = Firing)
	int32 Clips;

	//How many shots you make while firing status is true
	UPROPERTY()
	int32 CurrentShotsCount;

	//How many bullets in a clip we are using right now
	UPROPERTY()
	int32 CartridgesLeftInClip;

	//How many bullets can one clip handle
	UPROPERTY(EditAnywhere, Category = Firing)
	int32 ClipCapacity;

	//Amount of damage caused by this weapon
	UPROPERTY(EditAnywhere)
	float Damage;

public:

	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Fire();

	USoundBase * GetRandomShotSound();

	/** Weapon sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase * FirstShotSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	TArray<USoundBase*> ShotSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase * DrawSound;

	void Reload();

	void SetSimulatePhysics(bool SimulatePhyics);

	bool GetSimulatePhysics();

	void SetParentCharacter(AMainCharacter* NewParentCharacter);

	void SetFiringStatus(bool Firing);

	bool GetFiringStatus();

	UFUNCTION(BlueprintCallable, Category = "Cartridge State")
	int32 GetCartridgesInClipCount();

	UFUNCTION(BlueprintCallable, Category = "Cartridge State")
	bool IsClipFull();

	UFUNCTION(BlueprintCallable, Category = "Cartridge State")
	int32 GetClipSize();

	UFUNCTION(BlueprintCallable, Category = "Cartridge State")
	int32 GetClipCount();

	USkeletalMeshComponent* GetWeaponMesh();

	UFUNCTION()
	void OnPickUpBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnPickUpEndOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	/* IUsableInterface Implementation */
	virtual void OnUsed(ACharacter* User) override;

	virtual FString GetActionMessage() override;
	/* - - - - - - - - - - - - - - - - */
	
};
