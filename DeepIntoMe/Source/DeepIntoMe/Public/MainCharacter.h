// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "DIMGameMode.h"
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
	
	AWeapon* WeaponCopy;

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
	UPROPERTY(BlueprintReadWrite, Category = "Firing")
	bool bFiring;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Firing")
	bool bReloading;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Firing")
	bool bAiming;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement")
	bool bCrouching;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement")
	bool bRunning;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float XLookRate;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float YLookRate;
	
	// Test 'dead' flag
	UPROPERTY(ReplicatedUsing = OnRep_bTestIsDead)
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
	
	TSubclassOf<AWeapon> GetWeaponType();
	
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
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning();

	//Attaches weapon to a character and set it to not simulate physics and not to overlap
	void AttachWeaponToCharacter(AWeapon* NewWeapon);

	// Detaches weapon from character and set it to simulate physics and to overlap
	void DropWeapon();
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastDropWeapon();

	// Handles damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDeath();
	
	void CheckDeath(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCheckDeath(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
	// Add weapon to inventory
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void AddWeapon(AWeapon* NewWeapon);
	
	void AddWeaponFirst(AWeapon* NewWeapon);

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
	
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void DamageTakenEvent();
	
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void UpdateTeamColor(const EMultiplayerTeam Team);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInvokeColorChange();
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticastUpdateTeamColor(const EMultiplayerTeam Team);
	
	
	void SetSpectatorMode();
	
	UFUNCTION()
	void OnRep_bTestIsDead();
	
	bool IsAlive();
	
	UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
	void RebindWeapon();
	
};