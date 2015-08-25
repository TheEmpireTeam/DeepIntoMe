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

	UPROPERTY(EditAnywhere, Category = Health)
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

	UPROPERTY(BlueprintReadWrite, Category = Firing)
	bool bReloading;

	UPROPERTY(BlueprintReadWrite, Category = Firing)
	bool bAiming;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bCrouching;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bRunning;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float XLookRate;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float YLookRate;


public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

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

	//Stops firing process
	void StopFire();

	//Start reloading
	void Reload();

	void StartAiming();

	void StopAiming();

	void StartCrouching();

	void StopCrouching();

	void StartRunning();

	void StopRunning();

	void SetRunningStatus(bool Running);

	//Attaches weapon to a character and set it to not simulate physics and not to overlap
	void AttachWeaponToCharacter(AWeapon* NewWeapon);

	//Detaches weapon from character and set it to simulate physics and to overlap
	//Takes a new location for detached weapon
	void DetachWeaponFromCharacter(FTransform NewTransform);

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

	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* OtherActor);
	
	//Called when character is dying 
	void OnDying();

	//For using items that currently avalaible
	void UseItem();
	
};
