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
	virtual void Logout(AController* Exiting) override;

public:
	void UpdateDestroyCube(ARCN_RubikCube* RubikCube);
	void StartGame(ARCN_PlayerController* PressedPlayerController);
	void PlayerReady(ARCN_PlayerController* PressedPlayerController);

protected:
	virtual void LoginComplete(ARCN_PlayerController* NewPlayerController) override;
	void PromoteClientToHost(APlayerController* NewHostController);
	bool PlayerAllReadCheck();

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, TObjectPtr<ARCN_RubikCube>> PlayerCubeMap;

	UPROPERTY(visibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, bool> PlayerReadyMap;
};