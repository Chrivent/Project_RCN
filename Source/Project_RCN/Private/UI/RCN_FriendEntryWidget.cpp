// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_FriendEntryWidget.h"

#include "OnlineSubsystemTypes.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Utility/SessionManager.h"

void URCN_FriendEntryWidget::SetOnlineFriend(const TSharedPtr<FOnlineFriend>& InOnlineFriend)
{
	OnlineFriend = InOnlineFriend;
	NicknameText->SetText(FText::FromString(OnlineFriend->GetDisplayName()));

	if (!InviteButton->OnClicked.IsAlreadyBound(this, &URCN_FriendEntryWidget::InviteButtonReleasedHandle))
	{
		InviteButton->OnClicked.AddDynamic(this, &URCN_FriendEntryWidget::InviteButtonReleasedHandle);
	}
}

void URCN_FriendEntryWidget::InviteButtonReleasedHandle()
{
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->SendInviteSteamFriend(OnlineFriend);
	}
}
