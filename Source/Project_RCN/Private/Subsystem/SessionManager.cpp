// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SessionManager.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

void USessionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Found Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));

			SessionInterface.Pin()->OnCreateSessionCompleteDelegates.AddUObject(this, &USessionManager::OnCreateSessionCompleteHandle);
			SessionInterface.Pin()->OnDestroySessionCompleteDelegates.AddUObject(this, &USessionManager::OnDestroySessionCompleteHandle);
			SessionInterface.Pin()->OnFindSessionsCompleteDelegates.AddUObject(this, &USessionManager::OnFindSessionsCompleteHandle);
			SessionInterface.Pin()->OnJoinSessionCompleteDelegates.AddUObject(this, &USessionManager::OnJoinSessionCompleteHandle);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to get OnlineSessionInterface")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("OnlineSubsystem not found")));
	}
}

void USessionManager::JoinSession(const FOnlineSessionSearchResult& SearchResult) const
{
	if (!SessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("SessionInterface is invalid")));
		return;
	}
	
	// 세션 참가 요청
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Attempting to join session: %s"), *SearchResult.Session.OwningUserName));
	SessionInterface.Pin()->JoinSession(0, NAME_GameSession, SearchResult);
}

void USessionManager::CreateSession(const int32 NumPlayers) const
{
	if (!SessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("SessionInterface is invalid")));
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = NumPlayers; // 최대 플레이어 수
	SessionSettings.bShouldAdvertise = true; // 세션을 찾을 수 있도록 광고 설정
	SessionSettings.bUsesPresence = true; // 친구 목록에서 세션 표시 여부
	SessionSettings.bAllowInvites = true;  // 초대 가능
	SessionSettings.bAllowJoinViaPresence = true; // 친구 목록에서 "게임 참가" 버튼을 눌러 참가할 수 있도록 설정
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true; // 친구만 bAllowJoinViaPresence 기능을 사용할 수 있도록 설정.
	SessionSettings.bUseLobbiesIfAvailable = true; // 로비 시스템 우선 사용

	// Todo: 임시로 랜매치. 나중에 지워야함
	SessionSettings.bIsLANMatch = true;

	SessionInterface.Pin()->CreateSession(0, NAME_GameSession, SessionSettings);
}

void USessionManager::FindSessions()
{
	if (!SessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("SessionInterface is invalid")));
		return;
	}
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 1000;
	SessionSearch->PingBucketSize = 1000;

	// Todo: 임시로 랜매치. 나중에 지워야함
	SessionSearch->bIsLanQuery = true;

	SessionInterface.Pin()->FindSessions(0, SessionSearch.ToSharedRef());
}

void USessionManager::DestroySession() const
{
	if (!SessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("SessionInterface is invalid")));
		return;
	}
	
	SessionInterface.Pin()->DestroySession(NAME_GameSession);
}

void USessionManager::MigrateToHost(APlayerController* NewHostController)
{
	if (NewHostController)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("MigrateToHost New Host: %s"), *NewHostController->GetName()));
}

void USessionManager::OnCreateSessionCompleteHandle(const FName SessionName, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Session created successfully: %s"), *SessionName.ToString()));

		UGameplayStatics::OpenLevel(this, "LobbyLevel", true, "listen");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to create session: %s"), *SessionName.ToString()));
	}
}

void USessionManager::OnDestroySessionCompleteHandle(const FName SessionName, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Session destroyed successfully: %s"), *SessionName.ToString()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to destroy session: %s"), *SessionName.ToString()));
	}
}

void USessionManager::OnFindSessionsCompleteHandle(const bool bWasSuccessful)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("SessionInterface or SessionSearch is invalid")));
		return;
	}

	if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Found %d sessions"), SessionSearch->SearchResults.Num()));

		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[i];
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Session %d: Owner=%s, Ping=%d"), i, *Result.Session.OwningUserName, Result.PingInMs));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("No sessions found")));
	}
}

void USessionManager::OnJoinSessionCompleteHandle(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (!SessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("SessionInterface is invalid")));
		return;
	}

	switch (JoinResult)
	{
	case EOnJoinSessionCompleteResult::Success:
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Successfully joined session: %s"), *SessionName.ToString()));
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to join session: %s (Session is full)"), *SessionName.ToString()));
		return;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to join session: %s (Session does not exist)"), *SessionName.ToString()));
		return;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to join session: %s (Could Not Retrieve Address)"), *SessionName.ToString()));
		return;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to join session: %s (Already In Session)"), *SessionName.ToString()));
		return;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to join session: %s (Unknown error)"), *SessionName.ToString()));
		return;
	}

	// 세션 접속 정보 가져오기
	FString ConnectionInfo;
	if (!SessionInterface.Pin()->GetResolvedConnectString(SessionName, ConnectionInfo))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to get connection info for session: %s"), *SessionName.ToString()));
		return;
	}

	// 클라이언트를 서버로 이동
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Client traveling to: %s"), *ConnectionInfo));
		PlayerController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
	}
}
