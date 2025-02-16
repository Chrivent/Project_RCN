// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "RCN_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE TSharedPtr<FOnlineSessionSearch> GetSessionSearch() const { return SessionSearch; }

protected:
	virtual void Init() override;

public:
	void CreateSession(int32 NumPlayers);
	void FindSessions();
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();

protected:
	void OnCreateSessionCompleteHandle(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionCompleteHandle(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsCompleteHandle(bool bWasSuccessful);
	void OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);
	
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
