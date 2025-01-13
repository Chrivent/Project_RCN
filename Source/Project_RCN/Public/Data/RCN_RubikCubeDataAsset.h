// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_RubikCubeDataAsset.generated.h"

class UInputMappingContext;
class UInputAction;
enum class EStickerType : uint8;
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
	float PieceDistance;

	UPROPERTY(EditDefaultsOnly)
	float PieceSize;

	UPROPERTY(EditDefaultsOnly)
	float TurnSpeed;

	UPROPERTY(EditDefaultsOnly)
	float TurnTolerance;

	UPROPERTY(EditDefaultsOnly)
	TMap<EStickerType, TObjectPtr<UStaticMesh>> StickerMesh;

	UPROPERTY(EditDefaultsOnly)
	float StickerDistance;

	UPROPERTY(EditDefaultsOnly)
	float StickerSize;

	UPROPERTY(EditDefaultsOnly)
	int32 ScrambleTurnCount;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> RotateAction;
};
