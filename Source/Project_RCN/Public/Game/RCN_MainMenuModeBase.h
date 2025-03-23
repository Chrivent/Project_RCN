// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/RCN_GameModeBase.h"
#include "RCN_MainMenuModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_MainMenuModeBase : public ARCN_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual void LoginComplete(ARCN_PlayerController* NewPlayerController) override;
};
