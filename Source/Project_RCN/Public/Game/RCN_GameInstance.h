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
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;
	
	void CreateSession(const int32 NumPlayers) const;
	void FindSessions();
	void DestroySession() const;

	void MigrateToHost(APlayerController* NewHostController);

protected:
	void OnCreateSessionCompleteHandle(const FName SessionName, const bool bWasSuccessful);
	void OnDestroySessionCompleteHandle(const FName SessionName, const bool bWasSuccessful);
	void OnFindSessionsCompleteHandle(const bool bWasSuccessful);
	void OnJoinSessionCompleteHandle(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult);
	
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
