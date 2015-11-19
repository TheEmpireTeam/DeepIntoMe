// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"
#include "DeepIntoMeHUD.h"
#include "MainCharacter.h"
#include "DeepIntoMePlayerController.h"


void ADIMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	ResetScore();
	
	if (Role == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("New player connected!"));
	}	
	
	GiveName();
	AskTeamNumber();
}

void ADIMPlayerState::ResetScore()
{
	NumKills = 0;
	NumDeaths = 0;
}

void ADIMPlayerState::GiveName()
{
	if (Role < ROLE_Authority)
	{
		ServerGiveName();
	}
	else
	{
		const FString Suffix = FString::FromInt(FMath::RandRange(0, 999));
		SetPlayerName(FPlatformProcess::ComputerName() + Suffix);
	}
}

void ADIMPlayerState::ServerGiveName_Implementation()
{
	GiveName();
}

bool ADIMPlayerState::ServerGiveName_Validate()
{
	return true;
}

void ADIMPlayerState::OverrideName(const FString& Nickname)
{
	if (Role < ROLE_Authority)
	{
		UDIMGameInstance* GameInstance = Cast<UDIMGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance && !GameInstance->GetNickname().IsEmpty())
		{
			ServerOverrideName(GameInstance->GetNickname());
		}
	}
	else
	{
		SetPlayerName(Nickname);
	}
}

void ADIMPlayerState::ServerOverrideName_Implementation(const FString& Nickname)
{
	SetPlayerName(Nickname);
}

bool ADIMPlayerState::ServerOverrideName_Validate(const FString& Nickname)
{
	return true;
}

void ADIMPlayerState::AddKill()
{
	NumKills++;
}

void ADIMPlayerState::AddDeath()
{
	NumDeaths++;
}

void ADIMPlayerState::AskTeamNumber()
{
	// Ask player team on server side
	if (Role == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Server AskTeamNumber"));
	
		ADeepIntoMeGameState* GS = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
		SetTeamNumber(GS->GetNextPlayerTeamNumber());
	}
}

void ADIMPlayerState::ServerAskTeamNumber_Implementation()
{
	AskTeamNumber();
}

bool ADIMPlayerState::ServerAskTeamNumber_Validate()
{
	return true;
}

int32 ADIMPlayerState::GetTeamNumber()
{
	return TeamNumber;
}

int32 ADIMPlayerState::GetKills()
{
	return NumKills;
}
	
int32 ADIMPlayerState::GetDeaths()
{
	return NumDeaths;
}

void ADIMPlayerState::UpdatePlayerPawnColor()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, TEXT("UpdatePlayerPawnColor"));

	TArray<AActor*> PlayerPawns;
	UGameplayStatics::GetAllActorsOfClass(this, AMainCharacter::StaticClass(), PlayerPawns);

	// Split the player starts into two arrays for preferred and fallback spawns
	for (int32 i = 0; i < PlayerPawns.Num(); i++)
	{
		AMainCharacter* PlayerPawn = Cast<AMainCharacter>(PlayerPawns[i]);
		if (PlayerPawn && PlayerPawn->PlayerState == this)
		{
			PlayerPawn->ServerInvokeColorChange();
		}
	}

	/*for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(*Iterator);
		if (Controller && Controller->PlayerState == this)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Owning controller found!"));
				
			AMainCharacter* PlayerPawn = Cast<AMainCharacter>(Controller->GetPawn());
			if (PlayerPawn)
			{
				PlayerPawn->ServerInvokeColorChange();
			}
		}
	}*/
}

void ADIMPlayerState::NetMulticastUpdatePlayerPawnColor_Implementation()
{
	UpdatePlayerPawnColor();
}

bool ADIMPlayerState::NetMulticastUpdatePlayerPawnColor_Validate()
{
	return true;
}

// Called, when team number update received from server
void ADIMPlayerState::OnRep_TeamNumber()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, TEXT("Team Number Received: ") + FString::FromInt(TeamNumber));
	
	/*ADeepIntoMePlayerController* Controller = Cast<ADeepIntoMePlayerController>(GetWorld()->GetFirstPlayerController());
	if (Controller)
	{
		AMainCharacter* PlayerPawn = Cast<AMainCharacter>(Controller->GetPawn());
		if (PlayerPawn)
		{
			PlayerPawn->ServerInvokeColorChange();
		}
	}*/
}

void ADIMPlayerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
	
	// Update player color on server, real team color will be soon replicated to client
	UpdatePlayerPawnColor();
	
	/*if (Role < ROLE_Authority)
	{
		ServerSetTeamNumber(NewTeamNumber);
	}
	else
	{
		UpdatePlayerPawnColor();
	}*/
}

void ADIMPlayerState::ServerSetTeamNumber_Implementation(int32 NewTeamNumber)
{
	SetTeamNumber(NewTeamNumber);
}

bool ADIMPlayerState::ServerSetTeamNumber_Validate(int32 NewTeamNumber)
{
	return true;
}

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADIMPlayerState, NumKills);
	DOREPLIFETIME(ADIMPlayerState, NumDeaths);
	DOREPLIFETIME(ADIMPlayerState, TeamNumber);
	//DOREPLIFETIME_CONDITION(ADIMPlayerState, TeamNumber, COND_SkipOwner);
}
