// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "MainCharacter.generated.h"

UCLASS()
class AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	//Main camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	//Name of a socket for weapon
	UPROPERTY(EditAnywhere, Category = Weapon)
	FName SocketName;

	//Weapon type (!only for declearing type!)
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> WeaponType;

	//First Person Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	//Actual pointer to a weapon
	UPROPERTY()
	AWeapon* Weapon;

	UPROPERTY(Replicated, EditAnywhere, Category = Health)
	float Health;

	UPROPERTY(EditAnywhere, Category = Walking)
	float RunningSpeed;

	UPROPERTY(EditAnywhere, Category = Walking)
	float WalkingSpeed;

	//Items i can use right now
	TMap<FString, IUsableInterface*> Items;

	UPROPERTY(EditAnywhere)
	float BaseRate;

public:


	UPROPERTY(BlueprintReadWrite, Category = Firing)
	bool bFiring;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Firing)
	bool bReloading;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Firing)
	bool bAiming;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Movement)
	bool bCrouching;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Movement)
	bool bRunning;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float XLookRate;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float YLookRate;
	
	// Test 'dead' flag
	UPROPERTY(Replicated)
	bool bTestIsDead;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Move Camera in Y-axis
	void LookUp(float Value);

	//Move Camera in X-axis
	void LookRight(float Value);

	//Move character forward
	void MoveForward(float Value);

	//Move character right
	void MoveRight(float Value);

	//Start firing process
	void StartFire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartFire();

	//Stops firing process
	void StopFire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();

	//Start reloading
	void Reload();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	void StartAiming();
	
	void StopAiming();
	
	void SetAiming(bool Aiming);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetAiming(bool Aiming);

	void StartCrouching();

	void StopCrouching();
	
	void SetChourching(bool Crouching);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetChourching(bool Crouching);

	void StartRunning();

	void StopRunning();

	void SetRunningStatus(bool Running);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRunningStatus(bool Running);

	//Attaches weapon to a character and set it to not simulate physics and not to overlap
	void AttachWeaponToCharacter(AWeapon* NewWeapon);

	//Detaches weapon from character and set it to simulate physics and to overlap
	//Takes a new location for detached weapon
	//void DetachWeaponFromCharacter(FTransform NewTransform);
	
	void DropWeapon();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropWeapon();
	
	UFUNCTION(Client, Reliable)
	void ClientDropWeapon();

	//Handles damage
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//Add weapon to inventory
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void AddWeapon(AWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	USkeletalMeshComponent* GetWeaponMesh();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	AWeapon* GetWeapon();

	UFUNCTION(BlueprintCallable, Category = State)
	float GetHealth();

	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool IsMagazineEmpty();

	UFUNCTION()
	bool CanReload();

	UFUNCTION()
	void OnEndOverlap(AActor* OtherActor);
	
	// Called when character is dying 
	void OnDying();

	// For using items that currently avalaible
	void UseItem();
	
	
	void SetSpectatorMode();
	
};