// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_SessionListButtonWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Utility/SessionManager.h"

void URCN_SessionListButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	JoinSessionButton->OnReleased.AddDynamic(this, &URCN_SessionListButtonWidget::JoinSessionButtonReleasedHandle);
}

void URCN_SessionListButtonWidget::SetSessionSearchResult2(const FOnlineSessionSearchResult& InSessionSearchResult)
{
	SessionSearchResult = InSessionSearchResult;

	SessionNameText->SetText(FText::Format(NSLOCTEXT("SessionListButton", "OwnerFormat", "Owner: {0}"), FText::FromString(SessionSearchResult.Session.OwningUserName)));
	SessionPingText->SetText(FText::Format(NSLOCTEXT("SessionListButton", "PingFormat", "Ping: {0} ms"), SessionSearchResult.PingInMs));
}

void URCN_SessionListButtonWidget::JoinSessionButtonReleasedHandle()
{
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->JoinSession(SessionSearchResult);
	}
}
