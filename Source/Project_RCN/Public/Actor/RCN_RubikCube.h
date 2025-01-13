// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/StructHelper.h"
#include "RCN_RubikCube.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
enum class EAxisType : uint8;
class URCN_RubikCubeDataAsset;

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
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Spin(const FString& Command);
	void Scramble();
	void Solve();

	// 큐브 로직
protected:
	void TurnNext();
	void TurnCore(const FSignInfo& SignInfo);
	void UpdateTurnCore(const FSignInfo& SignInfo, FQuat TargetQuat);
	void GrabPieces(const FSignInfo& SignInfo);
	void ReleasePieces(const FSignInfo& SignInfo);
	static FMatrix GetRotationMatrix(const FSignInfo& SignInfo);
	void SortFacelet();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_RubikCubeDataAsset> RubikCubeDataAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PitchComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> YawComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> CoreComponent;
	
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> PieceMeshComponents;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FVector> PiecePositions;

	UPROPERTY(VisibleAnywhere)
	TArray<FSignInfo> SignInfos;

	UPROPERTY(VisibleAnywhere)
	TArray<FSignInfo> SignQueue;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsTurning : 1;

	UPROPERTY(VisibleAnywhere)
	uint8 bRequestedSolving : 1;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> StickerMeshComponents;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FVector> StickerPositions;
	
	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FColor> StickerColors;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> CenterOrderPositions;

	UPROPERTY(VisibleAnywhere)
	FString Facelet;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> FaceletOrderPositions;
	
	// 플레이 로직
	void SetControl() const;
	void HoldTriggered(const FInputActionValue& Value);
	void HoldCompleted(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> HoldAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsHolding : 1;
};
