// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_MainMenuModeBase.h"

#include "Actor/RCN_PlayerController.h"

void ARCN_MainMenuModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ARCN_PlayerController* NewPlayerController = Cast<ARCN_PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		NewPlayerController->CreateMainMenuWidget();
	}
}
