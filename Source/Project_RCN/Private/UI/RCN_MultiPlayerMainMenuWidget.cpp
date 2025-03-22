// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerMainMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "Project_RCN/Public/Utility/SessionManager.h"
#include "UI/RCN_SessionListButtonWidget.h"

void URCN_MultiPlayerMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::CreateSessionButtonReleasedHandle);
	FindSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::FindSessionButtonReleasedHandle);
	JoinConfirmButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::JoinConfirmButtonReleasedHandle);
	JoinCancelButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::JoinCancelButtonReleasedHandle);
	
	
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->CreatedSessionDelegate.AddUObject(this, &URCN_MultiPlayerMainMenuWidget::CreatedSessionsHandle);
		SessionManager->FoundSessionsDelegate.AddUObject(this, &URCN_MultiPlayerMainMenuWidget::FoundSessionsHandle);
	}
}

void URCN_MultiPlayerMainMenuWidget::VisibleOnNoticeOverlay()
{
	NoticeOverlay->SetVisibility(ESlateVisibility::Visible);
}

void URCN_MultiPlayerMainMenuWidget::CreateSessionButtonReleasedHandle()
{
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->CreateSession(4); // 최대 4인 세션 생성
	}
}

void URCN_MultiPlayerMainMenuWidget::FindSessionButtonReleasedHandle()
{
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->FindSessions();
	}
}

void URCN_MultiPlayerMainMenuWidget::JoinConfirmButtonReleasedHandle()
{
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->JoinSession(SessionSearchResult);
	}
}

void URCN_MultiPlayerMainMenuWidget::JoinCancelButtonReleasedHandle()
{
	NoticeOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void URCN_MultiPlayerMainMenuWidget::CreatedSessionsHandle()
{
	UGameplayStatics::OpenLevel(this, "GreenRoomLevel", true, "listen");
}

void URCN_MultiPlayerMainMenuWidget::FoundSessionsHandle(const TSharedPtr<FOnlineSessionSearch>& SessionSearch)
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->CreateSessionListButtonWidget(SessionSearch);
	}
}
