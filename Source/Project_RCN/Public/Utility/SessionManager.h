// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FCreatedSessionDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FFoundSessionsDelegate, const TSharedPtr<FOnlineSessionSearch>&)
DECLARE_MULTICAST_DELEGATE(FDestroyedSessionDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FReadSteamFriendsDelegate, const TArray<TSharedRef<FOnlineFriend>>&);

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API USessionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FCreatedSessionDelegate CreatedSessionDelegate;
	FFoundSessionsDelegate FoundSessionsDelegate;
	FDestroyedSessionDelegate DestroyedSessionDelegate;
	FReadSteamFriendsDelegate ReadSteamFriendsDelegate;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void CreateSession(const int32 NumPlayers) const;
	void FindSessions();
	void JoinSession(const FOnlineSessionSearchResult& SearchResult) const;
	void DestroySession(const APlayerController* PlayerController);
	void RequestReadSteamFriends() const;
	void SendInviteSteamFriend(const TSharedPtr<FOnlineFriend>& OnlineFriend) const;

protected:
	void OnCreateSessionCompleteHandle(const FName SessionName, const bool bWasSuccessful) const;
	void OnFindSessionsCompleteHandle(const bool bWasSuccessful) const;
	void OnJoinSessionCompleteHandle(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult) const;
	void OnDestroySessionCompleteHandle(const FName SessionName, const bool bWasSuccessful) const;
	
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//네트워크 로직
	UFUNCTION(Client, Reliable)
	void ClientRPC_DestroySession();
};
