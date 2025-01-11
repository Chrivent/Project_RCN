// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_RubikCubeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_RubikCubeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> PieceMesh;

	UPROPERTY(EditDefaultsOnly)
	float PieceSize;

	UPROPERTY(EditDefaultsOnly)
	float PieceDistance;

	UPROPERTY(EditDefaultsOnly)
	float TurnSpeed;

	UPROPERTY(EditDefaultsOnly)
	float TurnTolerance;
};
