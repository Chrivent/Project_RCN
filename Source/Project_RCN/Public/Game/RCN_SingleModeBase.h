// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RCN_GameModeBase.h"
#include "RCN_SingleModeBase.generated.h"

class URCN_GameModeBaseDataAsset;

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_SingleModeBase : public ARCN_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	virtual void FinishScramble() override;
};

