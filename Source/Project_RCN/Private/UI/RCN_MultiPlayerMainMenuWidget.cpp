// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerMainMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SessionManager.h"
#include "UI/RCN_SessionListEntryWidget.h"

void URCN_MultiPlayerMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::CreateSessionButtonReleasedHandle);
	FindSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::FindSessionButtonReleasedHandle);
	SessionListView->OnItemClicked().AddUObject(this, &URCN_MultiPlayerMainMenuWidget::SessionSelectedHandle);
	
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->OnCreatedSessionDelegate.AddUObject(this, &URCN_MultiPlayerMainMenuWidget::OnCreatedSessionsHandle);
		SessionManager->OnFoundSessionsDelegate.AddUObject(this, &URCN_MultiPlayerMainMenuWidget::OnFoundSessionsHandle);
	}
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

void URCN_MultiPlayerMainMenuWidget::OnCreatedSessionsHandle()
{
	UGameplayStatics::OpenLevel(this, "LobbyLevel", true, "listen");
}

void URCN_MultiPlayerMainMenuWidget::OnFoundSessionsHandle(const TSharedPtr<FOnlineSessionSearch>& SessionSearch)
{
	LastSessionSearch = SessionSearch.Get();
	SessionListView->ClearListItems();

	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->CreateSessionListEntryWidget(SessionListView, SessionSearch);
	}
}

void URCN_MultiPlayerMainMenuWidget::SessionSelectedHandle(UObject* SelectedItem) const
{
	if (const URCN_SessionListEntryWidget* SessionItem = Cast<URCN_SessionListEntryWidget>(SelectedItem))
	{
		const int32 SelectedIndex = SessionItem->GetSessionIndex();
		if (LastSessionSearch->SearchResults.IsValidIndex(SelectedIndex))
		{
			if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
			{
				SessionManager->JoinSession(LastSessionSearch->SearchResults[SelectedIndex]);
			}
		}
	}
}
