// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerGreenRoomWidget.h"

#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Utility/SessionManager.h"

void URCN_MultiPlayerGreenRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::BackButtonReleasedHandle);
	StartOrReadyButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::StartOrReadyButtonReleasedHandle);
	InviteButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerGreenRoomWidget::URCN_MultiPlayerGreenRoomWidget::InviteButtonReleasedHandle);
	
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->DestroyedSessionDelegate.AddUObject(this, &URCN_MultiPlayerGreenRoomWidget::DestroyedSessionsHandle);
		SessionManager->ReadSteamFriendsDelegate.AddUObject(this, &URCN_MultiPlayerGreenRoomWidget::ReadSteamFriendsHandle);
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

void URCN_MultiPlayerGreenRoomWidget::InviteButtonReleasedHandle()
{
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->RequestReadSteamFriends();
	}
}

void URCN_MultiPlayerGreenRoomWidget::DestroyedSessionsHandle() const
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->ClientTravel(TEXT("/Game/Level/MainMenuLevel"), TRAVEL_Absolute);
	}
}

void URCN_MultiPlayerGreenRoomWidget::ReadSteamFriendsHandle(const TArray<TSharedRef<FOnlineFriend>>& OnlineFriends) const
{
	for (const TSharedRef<FOnlineFriend>& Friend : OnlineFriends)
	{
		const FString DisplayName = Friend->GetDisplayName();
		const FString RealName = Friend->GetRealName();

		FString Msg = FString::Printf(TEXT("%s (%s)"), *DisplayName, *RealName);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, Msg);
	}

	FriendsScrollBox->ClearChildren();

	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		for (const auto OnlineFriend : OnlineFriends)
		{
			PlayerController->CreateFriendEntryWidget(FriendsScrollBox, OnlineFriend);
		}
	}
}
