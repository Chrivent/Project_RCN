// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RCN_SingleModeBase.generated.h"

class URCN_GameModeBaseDataAsset;
DECLARE_LOG_CATEGORY_EXTERN(LogRCNSingleModeBase, Log, All);

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_SingleModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARCN_SingleModeBase();
	
	virtual void BeginPlay() override;

protected:
	void SpawnPlayer();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_GameModeBaseDataAsset> GameModeBaseDataAsset;
};

