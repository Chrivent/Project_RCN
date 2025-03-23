// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/RCN_GameModeBase.h"
#include "RCN_MultiModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_MultiModeBase : public ARCN_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void LoginComplete(ARCN_PlayerController* NewPlayerController) override;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<ARCN_PlayerController>> PlayerControllers;
};
