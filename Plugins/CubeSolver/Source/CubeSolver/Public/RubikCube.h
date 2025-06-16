#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RubikCube.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRubikCube, Log, All);

class UBoxComponent;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

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
class CUBESOLVER_API ARubikCube : public AActor
{
	GENERATED_BODY()
	
public:	
	ARubikCube();

	FORCEINLINE bool IsSolved() const { return bIsSolved; }

	virtual void Tick(float DeltaSeconds) override;
	
	void Spin(const FString& Command);
	void Scramble();
	void Solve();
	FVector GetButtonPosition(UBoxComponent* ButtonBoxComponent);

	FFinishScramble FinishScrambleDelegate;
	FFinishSolve FinishSolveDelegate;

protected:
	void CreateStickerAndButton(UStaticMeshComponent* PieceMeshComponent, const FVector& Position, const EStickerType StickerType);
	void TurnNext();
	void TurnCore(const FSignInfo& SignInfo);
	void GrabPieces(const FSignInfo& SignInfo);
	void ReleasePieces(const FSignInfo& SignInfo);
	void ChangeFacelets(const FString& NewFacelets);
	static FMatrix GetRotationMatrix(const FSignInfo& SignInfo);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> PieceMesh;

	UPROPERTY(EditDefaultsOnly)
	float PieceDistance;

	UPROPERTY(EditDefaultsOnly)
	float PieceSize;

	UPROPERTY(EditDefaultsOnly)
	float TurnSpeed;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> StickerMesh;

	UPROPERTY(EditDefaultsOnly)
	TMap<EStickerType, TObjectPtr<UMaterial>> StickerMaterials;

	UPROPERTY(EditDefaultsOnly)
	float StickerDistance;

	UPROPERTY(EditDefaultsOnly)
	float StickerSize;

	UPROPERTY(EditDefaultsOnly)
	float ButtonSize;
	
	UPROPERTY(EditDefaultsOnly)
	float ButtonThickness;
	
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

	static const TArray<FSignInfo> SignInfos;
	static const TArray<FVector> FaceletOrderPositions;
	TQueue<FSignInfo> SignQueue;

	bool bIsUpdatingTurnCore = false;
	FSignInfo CurrentSignInfo;
	FQuat CurrentTargetQuat;

	// 네트워크 로직
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Facelets();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Spin(const FString& Command);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Spin(const FString& Command);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Scramble();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Solve();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_RenewalPattern(const FString& NewPattern);

	UPROPERTY(Replicated)
	uint8 bIsSolved : 1;

	UPROPERTY(ReplicatedUsing=OnRep_Facelets)
	FString Facelets;
};
