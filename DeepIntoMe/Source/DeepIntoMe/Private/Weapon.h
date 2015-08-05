// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UsableInterface.h"
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

	//Message that will appear on the screen
	UPROPERTY(EditAnywhere, Category = ActionSettings)
	FString ActionMessage;

public:

	//Weapon's collision for pick up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUpCollision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PickUpCollision;

	//Weapon's mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	bool bCanBePicked;

	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Fire();

	UFUNCTION()
	void OnPickUpBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnPickUpEndOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	//IUsableInterface overrided methods
	//-------------------------------------

	//Event for object to do some shit
	virtual void OnUsed(ACharacter* User) override;

	//A message that will be sent to HUD for using some shit
	virtual FString GetActionMessage() override;

	//-------------------------------------
	
};
