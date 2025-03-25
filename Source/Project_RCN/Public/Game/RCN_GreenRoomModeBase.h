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

public:
	ARCN_GreenRoomModeBase();
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Logout(AController* Exiting) override;

public:
	void StartGame(ARCN_PlayerController* PressedPlayerController);
	void PlayerReady(ARCN_PlayerController* PressedPlayerController);

protected:
	virtual void LoginComplete(ARCN_PlayerController* NewPlayerController) override;
	
	bool PlayerAllReadyCheck();

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, TObjectPtr<ARCN_RubikCube>> PlayerCubeMap;

	UPROPERTY(visibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, bool> PlayerReadyMap;

	UPROPERTY(visibleAnywhere)
	TMap<TObjectPtr<ARCN_PlayerController>, FQuat> PlayerTargetQuatMap;

	UPROPERTY(visibleAnywhere)
	float RotationAnglePerSecond;
};