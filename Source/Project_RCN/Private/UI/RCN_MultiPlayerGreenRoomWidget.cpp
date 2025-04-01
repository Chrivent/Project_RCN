// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerGreenRoomWidget.h"

#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Utility/SessionManager.h"

void URCN_MultiPlayerGreenRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle);
	StartOrReadyButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::StartOrReadyButtonReleasedHandle);
	
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->DestroyedSessionDelegate.AddUObject(this, &URCN_MultiPlayerGreenRoomWidget::DestroyedSessionsHandle);
	}
}

void URCN_MultiPlayerGreenRoomWidget::ChangePlayerReadyText(const int32 PlayerIndex, const FString& ReadyText) const
{
	switch (PlayerIndex)
	{
	case 0:
		if (IsValid(Player1ReadyText))
		{
			Player1ReadyText->SetText(FText::FromString(ReadyText));
		}
		break;
	case 1:
		if (IsValid(Player2ReadyText)) 
		{ 
			Player2ReadyText->SetText(FText::FromString(ReadyText));
		}
		break;
	case 2:
		if (IsValid(Player3ReadyText)) 
		{ 
			Player3ReadyText->SetText(FText::FromString(ReadyText));
		}
		break;
	case 3:
		if (IsValid(Player4ReadyText)) 
		{ 
			Player4ReadyText->SetText(FText::FromString(ReadyText));
		}
		break;
	default:
		break;
	}
}

void URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle()
{
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->DestroySession(GetOwningPlayer());
	}
}


void URCN_MultiPlayerGreenRoomWidget::StartOrReadyButtonReleasedHandle()
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->GreenRoomStartOrReady();
	}
}

void URCN_MultiPlayerGreenRoomWidget::DestroyedSessionsHandle() const
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->ClientTravel(TEXT("/Game/Level/MainMenuLevel"), TRAVEL_Absolute);
	}
}
