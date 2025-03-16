// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerGreenRoomWidget.h"

#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Game/RCN_LobbyModeBase.h"

void URCN_MultiPlayerGreenRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle);
}

void URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle()
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->RequestReturnToMenu();
	}
}
