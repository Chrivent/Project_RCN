// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/RCN_MultiModeBase.h"
#include "RCN_TestMultiModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_TestMultiModeBase : public ARCN_MultiModeBase
{
	GENERATED_BODY()

public:
	ARCN_TestMultiModeBase();

protected:
	virtual void Logout(AController* Exiting) override;
	
	virtual void LoginComplete(ARCN_PlayerController* NewPlayerController) override;
	virtual void SpinCube(ARCN_PlayerController* PlayerController, const FString& Command) override;

	UPROPERTY(VisibleAnywhere)
	int32 SpinnablePlayerNumber;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, int32> SpinCountMap;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, bool> ItemWidgetExistMap;
};
