// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/RCN_GameModeBase.h"
#include "RCN_LobbyModeBase.generated.h"

class ARCN_PlayerController;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_LobbyModeBase : public ARCN_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	TArray<int32> AvailablePlayerNumbers = { 0, 1, 2, 3 };
	TMap<TObjectPtr<ARCN_PlayerController>, int32> PlayerNumberMap;

	int32 GetAvailablePlayerNumber();
	void ReleasePlayerNumber(int32 PlayerNumber);
	
	TArray<FVector> CubeSpawnPosition = {
		FVector(1730.0f, -1000.0f, 0.0f),
		FVector(1730.0f, -300.0f, 0.0f),
		FVector(1730.0f, 300.0f, 0.0f),
		FVector(1730.0f, 1000.0f, 0.0f)
	};
};
