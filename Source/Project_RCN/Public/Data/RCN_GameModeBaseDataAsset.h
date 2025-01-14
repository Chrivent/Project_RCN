// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_GameModeBaseDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_GameModeBaseDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> DefaultPawnClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerController> PlayerControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RubikCubeClass;
};
