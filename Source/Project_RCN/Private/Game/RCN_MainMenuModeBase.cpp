// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_MainMenuModeBase.h"

#include "Actor/RCN_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Project_RCN/Project_RCN.h"

void ARCN_MainMenuModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ARCN_PlayerController* NewPlayerController = Cast<ARCN_PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		NewPlayerController->CreateMainMenu();
	}
}

void ARCN_MainMenuModeBase::StartLobby() const
{
	RCN_LOG(LogTemp, Log, TEXT("Starting Game..."));
	UGameplayStatics::OpenLevel(this, "MultiLevel", true, "listen");
}
