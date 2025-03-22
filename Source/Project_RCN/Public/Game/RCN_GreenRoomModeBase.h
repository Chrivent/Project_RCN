// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/RCN_GameModeBase.h"
#include "RCN_GreenRoomModeBase.generated.h"

class ARCN_RubikCube;
class ARCN_PlayerController;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_GreenRoomModeBase : public ARCN_GameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

public:
	void UpdateDestroyCube(ARCN_RubikCube* RubikCube);

protected:
	int32 GetAvailablePlayerNumber();
	void ReleasePlayerNumber(int32 PlayerNumber);
	
	UPROPERTY(VisibleAnywhere)
	TArray<int32> AvailablePlayerNumbers;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, int32> PlayerNumberMap;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, TObjectPtr<ARCN_RubikCube>> PlayerCubeMap;

	void PromoteClientToHost(APlayerController* NewHostController);
	void StartGame();
};