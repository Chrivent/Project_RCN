// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerMainMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Subsystem/SessionManager.h"

void URCN_MultiPlayerMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::CreateSessionButtonReleasedHandle);
	FindSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::FindSessionButtonReleasedHandle);
	JoinSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::JoinSessionButtonReleasedHandle);
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

void URCN_MultiPlayerMainMenuWidget::JoinSessionButtonReleasedHandle()
{
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		if (SessionManager->GetSessionSearch().IsValid() && SessionManager->GetSessionSearch()->SearchResults.Num() > 0)
		{
			SessionManager->JoinSession(SessionManager->GetSessionSearch()->SearchResults[0]); // 첫 번째 세션 참가
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("No available sessions to join"));
		}
	}
}
