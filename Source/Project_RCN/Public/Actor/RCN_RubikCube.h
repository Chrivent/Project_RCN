// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RCN_RubikCube.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class URCN_RubikCubeDataAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FSpinDelegate, const FString&)
DECLARE_MULTICAST_DELEGATE_OneParam(FPatternChangedDelegate, const FString&)
DECLARE_MULTICAST_DELEGATE(FFinishScramble)

UENUM(BlueprintType)
enum class EAxisType : uint8
{
	AxisX,
	AxisY,
	AxisZ
};

UENUM(BlueprintType)
enum class EStickerType : uint8
{
	Red,
	Orange,
	Yellow,
	Green,
	Blue,
	White
};

USTRUCT(BlueprintType)
struct FSignInfo
{
	GENERATED_BODY()

	FSignInfo(FString InSign = "L", const EAxisType InAxisType = EAxisType::AxisX, const int32 InLayer = -1, const bool InCCW = false, const int32 InTurnCount = 1)
	{
		Sign = InSign;
		AxisType = InAxisType;
		Layer = InLayer;
		CCW = InCCW;
		TurnCount = InTurnCount;
	}

	UPROPERTY(VisibleAnywhere)
	FString Sign;

	UPROPERTY(VisibleAnywhere)
	EAxisType AxisType;

	UPROPERTY(VisibleAnywhere)
	int32 Layer;

	UPROPERTY(VisibleAnywhere)
	uint8 CCW : 1;

	UPROPERTY(VisibleAnywhere)
	int32 TurnCount;
};

UCLASS()
class PROJECT_RCN_API ARCN_RubikCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARCN_RubikCube();

	FORCEINLINE FString GetPattern() const { return Pattern; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Spin(const FString& Command);
	void Scramble();
	void Solve();
	void ChangePattern(const FString& NewPattern);
	FVector GetStickerPosition(UStaticMeshComponent* StickerMeshComponent);

	FSpinDelegate SpinDelegate;
	FPatternChangedDelegate PatternChangedDelegate;
	FFinishScramble FinishScrambleDelegate;

protected:
	void TurnNext();
	void TurnCore(const FSignInfo& SignInfo);
	void UpdateTurnCore(const FSignInfo& SignInfo, const FQuat& TargetQuat);
	void GrabPieces(const FSignInfo& SignInfo);
	void ReleasePieces(const FSignInfo& SignInfo);
	static FMatrix GetRotationMatrix(const FSignInfo& SignInfo);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_RubikCubeDataAsset> RubikCubeDataAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultComponent;

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
	uint8 bIsScrambling : 1;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> StickerMeshComponents;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FVector> StickerPositions;

	UPROPERTY(VisibleAnywhere)
	FString Pattern;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> PatternOrderPositions;
};
