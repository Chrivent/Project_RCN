// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_RubikCubeDataAsset.generated.h"

enum class EStickerType : uint8;
class UInputMappingContext;
class UInputAction;
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
	TObjectPtr<UStaticMesh> StickerMesh;

	UPROPERTY(EditDefaultsOnly)
	TMap<EStickerType, TObjectPtr<UMaterial>> StickerMaterials;

	UPROPERTY(EditDefaultsOnly)
	float StickerDistance;

	UPROPERTY(EditDefaultsOnly)
	float StickerSize;

	UPROPERTY(EditDefaultsOnly)
	int32 ScrambleTurnCount;

	UPROPERTY(EditDefaultsOnly)
	float ButtonSize;
	
	UPROPERTY(EditDefaultsOnly)
	float ButtonThickness;
};
