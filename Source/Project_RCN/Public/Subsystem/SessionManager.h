// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCreatedSessionDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFoundSessionsDelegate, const TSharedPtr<FOnlineSessionSearch>& SessionSearch)

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API USessionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FORCEINLINE TSharedPtr<FOnlineSessionSearch> GetSessionSearch() const { return SessionSearch; }

	FOnCreatedSessionDelegate OnCreatedSessionDelegate;
	FOnFoundSessionsDelegate OnFoundSessionsDelegate;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void JoinSession(const FOnlineSessionSearchResult& SearchResult) const;
	void CreateSession(const int32 NumPlayers) const;
	void FindSessions();
	void DestroySession() const;

	static void MigrateToHost(const APlayerController* NewHostController);

protected:
	void OnCreateSessionCompleteHandle(const FName SessionName, const bool bWasSuccessful);
	void OnDestroySessionCompleteHandle(const FName SessionName, const bool bWasSuccessful);
	void OnFindSessionsCompleteHandle(const bool bWasSuccessful);
	void OnJoinSessionCompleteHandle(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult);
	
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
