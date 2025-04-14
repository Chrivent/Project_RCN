// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_FriendEntryWidget.h"

#include "OnlineSubsystemTypes.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URCN_FriendEntryWidget::Setup(const TSharedRef<FOnlineFriend>& OnlineFriend)
{
	NicknameText->SetText(FText::FromString(OnlineFriend->GetDisplayName()));
	FriendId = OnlineFriend->GetUserId();

	InviteButton->OnClicked.AddDynamic(this, &URCN_FriendEntryWidget::InviteButtonReleasedHandle);
}

void URCN_FriendEntryWidget::InviteButtonReleasedHandle()
{
	
}
