// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/StructHelper.h"
#include "RCN_RubikCube.generated.h"

enum class EAxisType : uint8;
class URCN_RubikCubeDataAsset;

DECLARE_LOG_CATEGORY_EXTERN(RubikCube, Log, All);

UCLASS()
class PROJECT_RCN_API ARCN_RubikCube : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARCN_RubikCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Spin(const FString& Command);
	void Scramble();

protected:
	void TurnNext();
	void TurnCore(const FSignInfo& SignInfo);
	void UpdateTurnCore(const FSignInfo& SignInfo, FQuat TargetRotator);
	void GrabPieces(const FSignInfo& SignInfo);
	void ReleasePieces(const FSignInfo& SignInfo);
	static FMatrix GetRotationMatrix(const FSignInfo& SignInfo);
	void SortFacelet();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_RubikCubeDataAsset> RubikCubeDataAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> CoreComponent;
	
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> PieceMeshComponents;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FVector> PieceMeshPositions;

	UPROPERTY(VisibleAnywhere)
	TArray<FSignInfo> SignInfos;

	UPROPERTY(VisibleAnywhere)
	TArray<FSignInfo> SignQueue;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsTurning : 1;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> StickerMeshComponents;

	UPROPERTY(VisibleAnywhere)
	FString Facelet;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FVector> StickerPositions;
	
	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FColor> StickerColors;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> CenterOrderPositions;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> FaceletOrderPositions;
};
