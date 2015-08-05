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

	//An item that i can use right now
	IUsableInterface* UsableItem;

	//Actual pointer to a weapon
	AWeapon* Weapon;

	float BaseRate;

	bool bFiring;


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

	void StartFire();

	void StopFire();

	//Add weapon to inventory
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void AddWeapon(AWeapon* NewWeapon);

	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* OtherActor);

	void UseItem();
	
};
