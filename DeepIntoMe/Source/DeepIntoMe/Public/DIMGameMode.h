// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
//#include "DIMPlayerState.h"
#include "DIMGameMode.generated.h"

UENUM(BlueprintType)
enum class EMultiplayerTeam : uint8 {
	Spectator,
	Earth,
	Bailosh,
	Aliens
};

UENUM(BlueprintType)
enum class EGamePlayMode : uint8 {
	Deathmatch,
	TeamDeathmatch,
	Dominate,
	CaptureTheFlag
};

UCLASS()
class DEEPINTOME_API ADIMGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	EGamePlayMode DefaultPlayMode;
	
public:
	ADIMGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	virtual void RestartPlayer(class AController* NewPlayer) override;
	
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

	virtual void StartNewPlayer(APlayerController* NewPlayer) override;
	
	virtual void StartMatch();
	
	virtual void PostLogin(APlayerController * NewPlayer) override;
	
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	
	//virtual bool CanDealDamage(class ADIMPlayerState* DamageCauser, class ADIMPlayerState* DamagedPlayer) const;
	
	EGamePlayMode GetDefaultPlayMode();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static FString GamePlayModeToString(EGamePlayMode Mode);
};
