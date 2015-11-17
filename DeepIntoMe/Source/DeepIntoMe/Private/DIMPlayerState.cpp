// Fill out your copyright notice in the Description page of Project Settings.

#include "DeepIntoMe.h"
#include "DIMPlayerState.h"
#include "DeepIntoMeGameState.h"
#include "DIMGameInstance.h"
#include "DeepIntoMeHUD.h"
#include "MainCharacter.h"


void ADIMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	UDIMGameInstance* GameInstance = Cast<UDIMGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance && !GameInstance->GetNickname().IsEmpty())
	{
		FString Nick = GameInstance->GetNickname();
	
		if (Role < ROLE_Authority)
		{
			ServerOverrideName(Nick);
		}
		else
		{
			SetPlayerName(Nick);
		}
	}
	else
	{
		GiveName();
	}
	
	ResetScore();
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
	if (Role < ROLE_Authority)
	{
		ServerAskTeamNumber();
	}
	else
	{
		ADeepIntoMeGameState* GS = Cast<ADeepIntoMeGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			TeamNumber = GS->GetNextPlayerTeamNumber();
			
			TArray<AActor*> PlayerPawns;
			UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), PlayerPawns);
			
			for (int32 i = 0; i < PlayerPawns.Num(); i++)
			{
				AMainCharacter* Character = Cast<AMainCharacter>(PlayerPawns[i]);
				if (Character && Character->PlayerState == this)
				{
					Character->UpdateTeamColor();
					break;
				}
			}
		}
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

void ADIMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADIMPlayerState, NumKills);
	DOREPLIFETIME(ADIMPlayerState, NumDeaths);
	DOREPLIFETIME(ADIMPlayerState, TeamNumber);
}
