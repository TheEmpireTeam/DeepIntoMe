// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UsableInterface.h"
#include "Projectile.h"
#include "Weapon.generated.h"


UENUM()
enum class EWeaponState
{
	Idle,
	Firing,
	Equipping,
	Reloading
};


class AMainCharacter;

UCLASS()
class AWeapon : public AActor, public IUsableInterface
{
	GENERATED_BODY()

private:

	const int MAX_AIM_DISTANCE = 100000;
	

	AMainCharacter* ParentCharacter;

	// Message that will appear on the screen
	UPROPERTY(EditDefaultsOnly, Category = "Action Settings")
	FString ActionMessage;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	TSubclassOf<AProjectile> ProjectileType;

	// Name of a socket for Firing
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	FName FireSocketName;

	// Weapon's collision for pick up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickUp Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* PickUpCollision;

	// Weapon's mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	
	/* Firing */
	
	// Amount of bullets per second
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float FireRate;

	// Maximum offset for firing 
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float MaxOffset;

	// Offset rate for firing 
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float OffsetRate;

	// Does weapon simulate physics
	UPROPERTY()
	bool bSimulatePhysics;

	// Is weapon firing right now
	UPROPERTY()
	bool bFiring;

	UPROPERTY(EditDefaultsOnly, Category = "Info")
	FString InventoryName;

	// Amount of damage caused by this weapon
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	bool bIsAutomatic;
	
	
	float TimeBetweenShots;
	
	float LastFireTime;
	
	FTimerHandle FiringHandleTimer;

public:

	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HandleFiring();
	
	void PlayShootSound();

	USoundBase * GetRandomShotSound();

	// Weapon sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TArray<USoundBase*> ShotSounds;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase * OutOfAmmoSound;

	void Reload();

	void SetSimulatePhysics(bool SimulatePhyics);

	bool GetSimulatePhysics();

	void SetParentCharacter(AMainCharacter* NewParentCharacter);

	USkeletalMeshComponent* GetWeaponMesh();

	UFUNCTION()
	void OnPickUpBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnPickUpEndOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	/* Clip state */
	// Amount of clips
	UPROPERTY(EditAnywhere, Category = "Firing")
	int32 AmmoLeftTotal;

	// How many shots you have made from fire start, used for dispersion implementation
	int32 CurrentShotsCount;

	// How many bullets in a clip we are using right now
	int32 AmmoLeftInClip;

	// How many bullets can one clip handle
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 ClipCapacity;
	
	
	UFUNCTION(BlueprintCallable, Category = "Clip State")
	int32 GetAmmoInClipCount();

	UFUNCTION(BlueprintCallable, Category = "Clip State")
	bool IsClipFull();
	
	bool CanReload();

	UFUNCTION(BlueprintCallable, Category = "Clip State")
	int32 GetClipSize();

	UFUNCTION(BlueprintCallable, Category = "Clip State")
	int32 GetRemainingAmmoCount();
	
	// Function part
	void StartFire();
	
	void StopFire();

	bool GetFiringStatus();
	

public:

	/* IUsableInterface Implementation */
	virtual void OnUsed(ACharacter* User) override;

	virtual FString GetActionMessage() override;
	
	UFUNCTION(BlueprintCallable, Category = "Info")
	virtual FString GetUsableName() override;
	/* - - - - - - - - - - - - - - - - */
	
};