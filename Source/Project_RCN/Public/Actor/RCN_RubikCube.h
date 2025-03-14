// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RCN_RubikCube.generated.h"

class UBoxComponent;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class URCN_RubikCubeDataAsset;

DECLARE_MULTICAST_DELEGATE(FFinishScramble)
DECLARE_MULTICAST_DELEGATE(FFinishSolve)

UENUM(BlueprintType)
enum class ECubeAxisType : uint8
{
	X,
	Y,
	Z
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

	FSignInfo(FString InSign = "L", const ECubeAxisType InCubeAxisType = ECubeAxisType::X, const int32 InLayer = -1, const bool InCCW = false, const int32 InTurnCount = 1)
	{
		Sign = InSign;
		CubeAxisType = InCubeAxisType;
		Layer = InLayer;
		CCW = InCCW;
		TurnCount = InTurnCount;
	}

	UPROPERTY(VisibleAnywhere)
	FString Sign;

	UPROPERTY(VisibleAnywhere)
	ECubeAxisType CubeAxisType;

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
	FVector GetButtonPosition(UBoxComponent* ButtonBoxComponent);

	FFinishScramble FinishScrambleDelegate;
	FFinishSolve FinishSolveDelegate;

protected:
	void CreateStickerAndButton(UStaticMeshComponent* PieceMeshComponent, const float PieceSize, const FVector& Position, const EStickerType StickerType);
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
	TArray<TObjectPtr<UStaticMeshComponent>> StickerMeshComponents;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UStaticMeshComponent>, FVector> StickerPositions;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UBoxComponent>> ButtonBoxComponents;

	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UBoxComponent>, FVector> ButtonPositions;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsTurning : 1;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsScrambling : 1;

	UPROPERTY(VisibleAnywhere)
	FString Pattern;

	static const TArray<FSignInfo> SignInfos;
	static const TArray<FVector> PatternOrderPositions;
	TQueue<FSignInfo> SignQueue;

	// 네트워크 로직
	UFUNCTION(Server, Reliable)
	void ServerRPC_Spin(const FString& Command);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Spin(const FString& Command);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Scramble();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Solve();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_ChangePattern(const FString& NewPattern);
};
