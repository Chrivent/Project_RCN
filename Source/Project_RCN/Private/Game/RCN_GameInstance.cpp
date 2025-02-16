// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Game/RCN_MainMenuModeBase.h"

void URCN_GameInstance::Init()
{
	Super::Init();

	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Blue,
					FString::Printf(TEXT("Found Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString())
				);
			}

			SessionInterface.Pin()->OnCreateSessionCompleteDelegates.AddUObject(this, &URCN_GameInstance::OnCreateSessionCompleteHandle);
			SessionInterface.Pin()->OnDestroySessionCompleteDelegates.AddUObject(this, &URCN_GameInstance::OnDestroySessionCompleteHandle);
			SessionInterface.Pin()->OnFindSessionsCompleteDelegates.AddUObject(this, &URCN_GameInstance::OnFindSessionsCompleteHandle);
			SessionInterface.Pin()->OnJoinSessionCompleteDelegates.AddUObject(this, &URCN_GameInstance::OnJoinSessionCompleteHandle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get OnlineSessionInterface"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem not found"));
	}
}

void URCN_GameInstance::CreateSession(const int32 NumPlayers)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is invalid"));
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true; // LAN 게임 여부
	SessionSettings.NumPublicConnections = NumPlayers; // 최대 플레이어 수
	SessionSettings.bShouldAdvertise = true; // 세션을 찾을 수 있도록 광고 설정
	SessionSettings.bUsesPresence = true; // 친구 목록에서 세션 표시 여부

	SessionInterface.Pin()->CreateSession(0, NAME_GameSession, SessionSettings);
}

void URCN_GameInstance::FindSessions()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is invalid"));
		return;
	}
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10;
	SessionSearch->PingBucketSize = 50;

	SessionInterface.Pin()->FindSessions(0, SessionSearch.ToSharedRef());
}

void URCN_GameInstance::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is invalid"));
		return;
	}
	
	// 세션 참가 요청
	UE_LOG(LogTemp, Log, TEXT("Attempting to join session: %s"), *SearchResult.Session.OwningUserName);
	SessionInterface.Pin()->JoinSession(0, NAME_GameSession, SearchResult);
}

void URCN_GameInstance::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is invalid"));
		return;
	}
	
	SessionInterface.Pin()->DestroySession(NAME_GameSession);
}

void URCN_GameInstance::OnCreateSessionCompleteHandle(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session created successfully: %s"), *SessionName.ToString());

		// 세션을 성공적으로 생성하면 게임 시작
		if (ARCN_MainMenuModeBase* MainMenuModeBase = Cast<ARCN_MainMenuModeBase>(GetWorld()->GetAuthGameMode()))
		{
			MainMenuModeBase->StartLobby();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session: %s"), *SessionName.ToString());
	}
}

void URCN_GameInstance::OnDestroySessionCompleteHandle(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session destroyed successfully: %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session: %s"), *SessionName.ToString());
	}
}

void URCN_GameInstance::OnFindSessionsCompleteHandle(bool bWasSuccessful)
{
	if (!SessionSearch.IsValid() || !SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionSearch or SessionInterface is invalid"));
		return;
	}

	if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());

		for (int32 Index = 0; Index < SessionSearch->SearchResults.Num(); ++Index)
		{
			const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[Index];
			UE_LOG(LogTemp, Log, TEXT("Session %d: Owner=%s, Ping=%d"), 
				Index, *Result.Session.OwningUserName, Result.PingInMs);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
	}
}

void URCN_GameInstance::OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is invalid in OnJoinSessionCompleteHandle"));
		return;
	}

	switch (JoinResult)
	{
	case EOnJoinSessionCompleteResult::Success:
		UE_LOG(LogTemp, Log, TEXT("Successfully joined session: %s"), *SessionName.ToString());
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session: %s (Session is full)"), *SessionName.ToString());
		return;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session: %s (Session does not exist)"), *SessionName.ToString());
		return;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session: %s (Unknown error)"), *SessionName.ToString());
		return;
	}

	// 세션 접속 정보 가져오기
	FString ConnectionInfo;
	if (!SessionInterface.Pin()->GetResolvedConnectString(SessionName, ConnectionInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get connection info for session: %s"), *SessionName.ToString());
		return;
	}

	// 클라이언트를 서버로 이동
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("Client traveling to: %s"), *ConnectionInfo);
		PlayerController->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
	}
}
