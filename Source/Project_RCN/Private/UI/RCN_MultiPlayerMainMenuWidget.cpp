// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MultiPlayerMainMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Game/RCN_GameInstance.h"

void URCN_MultiPlayerMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::CreateSessionButtonReleasedHandle);
	FindSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::FindSessionButtonReleasedHandle);
	JoinSessionButton->OnReleased.AddDynamic(this, &URCN_MultiPlayerMainMenuWidget::JoinSessionButtonReleasedHandle);
}

void URCN_MultiPlayerMainMenuWidget::CreateSessionButtonReleasedHandle()
{
	if (URCN_GameInstance* GameInstance = Cast<URCN_GameInstance>(GetGameInstance()))
	{
		GameInstance->CreateSession(2); // 최대 2인 세션 생성
	}
}

void URCN_MultiPlayerMainMenuWidget::FindSessionButtonReleasedHandle()
{
	if (URCN_GameInstance* GameInstance = Cast<URCN_GameInstance>(GetGameInstance()))
	{
		GameInstance->FindSessions();
	}
}

void URCN_MultiPlayerMainMenuWidget::JoinSessionButtonReleasedHandle()
{
	if (URCN_GameInstance* GameInstance = Cast<URCN_GameInstance>(GetGameInstance()))
	{
		if (GameInstance->GetSessionSearch().IsValid() && GameInstance->GetSessionSearch()->SearchResults.Num() > 0)
		{
			GameInstance->JoinSession(GameInstance->GetSessionSearch()->SearchResults[0]); // 첫 번째 세션 참가
		}
	}
}
