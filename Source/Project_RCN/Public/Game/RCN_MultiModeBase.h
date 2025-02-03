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
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

public:
	virtual void FinishScramble() override;
};
