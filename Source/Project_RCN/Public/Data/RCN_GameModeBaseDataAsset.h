// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_GameModeBaseDataAsset.generated.h"

class ARCN_RubikCube;
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
	TSubclassOf<ARCN_RubikCube> RubikCubeClass;
	
	UPROPERTY(EditDefaultsOnly)
	float CubeStartDistance;

	UPROPERTY(EditDefaultsOnly)
	FRotator CubeStartRotation;
	
	UPROPERTY(EditDefaultsOnly)
	FVector GreenRoomCubeSpawnPosition[4];
	
	UPROPERTY(EditDefaultsOnly)
	float CubeDestroySpeed;
};
