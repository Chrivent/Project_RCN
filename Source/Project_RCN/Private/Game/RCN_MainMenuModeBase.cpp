// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_MainMenuModeBase.h"

#include "Actor/RCN_PlayerController.h"

void ARCN_MainMenuModeBase::LoginComplete(ARCN_PlayerController* NewPlayerController)
{
	Super::LoginComplete(NewPlayerController);

	NewPlayerController->CreateMainMenuWidget();
}
