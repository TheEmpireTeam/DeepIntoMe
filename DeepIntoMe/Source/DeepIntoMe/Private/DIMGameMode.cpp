// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMGameMode.h"
#include "DIMPlayerStart.h"
#include "DIMPlayerState.h"

	
AActor* ADIMGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<ADIMPlayerStart*> PreferredSpawns;

	// Get all playerstart objects in level
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	// Split the player starts into two arrays for preferred and fallback spawns
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		ADIMPlayerStart* TestStart = Cast<ADIMPlayerStart>(PlayerStarts[i]);

		if (TestStart)
		{
			if (Player)
			{
				
			}
		
			/*ADIMPlayerState* PS = Cast<ADIMPlayerState>(Player->GetPawn()->PlayerState);
			if (PS && PS->GetTeamNumber() == TestStart->TeamNumber)
			{
				PreferredSpawns.Add(TestStart);
			}*/
		}
	}
	
	/*if (PreferredSpawns.Num() > 0)
	{
		return PreferredSpawns[FMath::RandRange(0, PreferredSpawns.Num() - 1)];
	}*/
	
	return Super::ChoosePlayerStart_Implementation(Player);
}


void ADIMGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	// Fallback to PlayerStart picking if team spawning is disabled or we're trying to spawn a bot.
	/*if (!bSpawnAtTeamPlayer || (NewPlayer->PlayerState && NewPlayer->PlayerState->bIsABot))
	{
		Super::RestartPlayer(NewPlayer);
		return;
	}

	// Look for a live player to spawn next to
	FVector SpawnOrigin = FVector::ZeroVector;
	FRotator StartRotation = FRotator::ZeroRotator;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		ASCharacter* MyCharacter = Cast<ASCharacter>(*It);
		if (MyCharacter && MyCharacter->IsAlive())
		{
			// Get the origin of the first player we can find
			SpawnOrigin = MyCharacter->GetActorLocation();
			StartRotation = MyCharacter->GetActorRotation();
			break;
		}
	}

	// No player is alive (yet) - spawn using one of the PlayerStarts
	if (SpawnOrigin == FVector::ZeroVector)
	{
		Super::RestartPlayer(NewPlayer);
		return;
	}

	// Get a point on the nav mesh near the other player
	FVector StartLocation = UNavigationSystem::GetRandomPointInNavigableRadius(NewPlayer, SpawnOrigin, 250.0f);

	// Try to create a pawn to use of the default class for this player
	if (NewPlayer->GetPawn() == nullptr && GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(GetDefaultPawnClassForController(NewPlayer), StartLocation, StartRotation, SpawnInfo);
		if (ResultPawn == nullptr)
		{
			UE_LOG(LogGameMode, Warning, TEXT("Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(DefaultPawnClass), &StartLocation);
		}
		NewPlayer->SetPawn(ResultPawn);
	}

	if (NewPlayer->GetPawn() == nullptr)
	{
		NewPlayer->FailedToSpawnPawn();
	}
	else
	{
		NewPlayer->Possess(NewPlayer->GetPawn());

		// If the Pawn is destroyed as part of possession we have to abort
		if (NewPlayer->GetPawn() == nullptr)
		{
			NewPlayer->FailedToSpawnPawn();
		}
		else
		{
			// Set initial control rotation to player start's rotation
			NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

			FRotator NewControllerRot = StartRotation;
			NewControllerRot.Roll = 0.f;
			NewPlayer->SetControlRotation(NewControllerRot);

			SetPlayerDefaults(NewPlayer->GetPawn());
		}
	}*/
}

FString ADIMGameMode::InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal)
{
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	/*FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	ADIMPlayerState* NewPlayerState = Cast<ADIMPlayerState>(NewPlayerController->PlayerState);
	if (NewPlayerState)
	{
		//NewPlayerState->SetTeamNumber(0);
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, L"NewPlayerState");
	}

	return Result;*/
}

