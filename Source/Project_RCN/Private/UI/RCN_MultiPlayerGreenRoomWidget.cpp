// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerGreenRoomWidget.h"

#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Utility/SessionManager.h"

void URCN_MultiPlayerGreenRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle);
	
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->OnDestroyedSessionDelegate.AddUObject(this, &URCN_MultiPlayerGreenRoomWidget::OnDestroyedSessionsHandle);
	}
}

void URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle()
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->RequestReturnToMenu();
	}
}

void URCN_MultiPlayerGreenRoomWidget::OnDestroyedSessionsHandle()
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->ClientTravel(TEXT("/Game/Level/MainMenuLevel"), TRAVEL_Absolute);
	}
}
