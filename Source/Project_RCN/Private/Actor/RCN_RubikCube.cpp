// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_RubikCube.h"

#include "Data/RCN_RubikCubeDataAsset.h"
#include "Util/EnumHelper.h"
#include "Util/StructHelper.h"

// Sets default values
ARCN_RubikCube::ARCN_RubikCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<URCN_RubikCubeDataAsset> RubikCubeDataAssetRef(TEXT("/Script/Project_RCN.RCN_RubikCubeDataAsset'/Game/Data/DA_RubikCube.DA_RubikCube'"));
	if (RubikCubeDataAssetRef.Object)
	{
		RubikCubeDataAsset = RubikCubeDataAssetRef.Object;
	}
	else
	{
		return;
	}

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;

	CoreComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CoreComponent"));
	CoreComponent->SetupAttachment(RootComponent);

	const float PieceDistance = RubikCubeDataAsset->PieceDistance;
	const float PieceSize = RubikCubeDataAsset->PieceSize;

	const float StickerDistance = RubikCubeDataAsset->StickerDistance;
	const float StickerSize = RubikCubeDataAsset->StickerSize;
	
	for (int32 Z = -1; Z <= 1; Z++)
	{
		for (int32 Y = -1; Y <= 1; Y++)
		{
			for (int32 X = -1; X <= 1; X++)
			{
				if (X == 0 && Y == 0 && Z == 0)
				{
					continue;
				}

				UStaticMeshComponent* PieceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("PieceComponent [%d, %d, %d]"), X, Y, Z));
				
				PieceMeshComponent->SetupAttachment(RootComponent);
				PieceMeshComponent->SetRelativeLocation(FVector(X * PieceDistance, Y * PieceDistance, Z * PieceDistance));
				PieceMeshComponent->SetRelativeScale3D(FVector(PieceSize));
				PieceMeshComponent->SetStaticMesh(RubikCubeDataAsset->PieceMesh);

				if (X == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Orange [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(-StickerDistance, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Orange]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
				}

				if (X == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Red [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(StickerDistance, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f).Quaternion());
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Red]);
					
					StickerMeshComponents.Emplace(StickerMeshComponent);
				}

				if (Y == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Green [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, -StickerDistance, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f).Quaternion());
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Green]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
				}

				if (Y == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Blue [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, StickerDistance, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f).Quaternion());
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Blue]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
				}

				if (Z == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent White [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -StickerDistance));
					StickerMeshComponent->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f).Quaternion());
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::White]);
					
					StickerMeshComponents.Emplace(StickerMeshComponent);
				}

				if (Z == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Yellow [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, StickerDistance));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Yellow]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
				}

				PieceMeshComponents.Emplace(PieceMeshComponent, FVector(X, Y, Z));
			}
		}
	}
	
	SignInfos.Emplace("L", EAxisType::AxisX, -1, false, 1);
	SignInfos.Emplace("L'", EAxisType::AxisX, -1, true, 1);
	SignInfos.Emplace("L2", EAxisType::AxisX, -1, false, 2);
	
	SignInfos.Emplace("M", EAxisType::AxisX, 0, true, 1);
	SignInfos.Emplace("M'", EAxisType::AxisX, 0, false, 1);
	SignInfos.Emplace("M2", EAxisType::AxisX, 0, true, 2);
	
	SignInfos.Emplace("R", EAxisType::AxisX, 1, true, 1);
	SignInfos.Emplace("R'", EAxisType::AxisX, 1, false, 1);
	SignInfos.Emplace("R2", EAxisType::AxisX, 1, true, 2);

	SignInfos.Emplace("B", EAxisType::AxisY, -1, false, 1);
	SignInfos.Emplace("B'", EAxisType::AxisY, -1, true, 1);
	SignInfos.Emplace("B2", EAxisType::AxisY, -1, false, 2);
	
	SignInfos.Emplace("S", EAxisType::AxisY, 0, true, 1);
	SignInfos.Emplace("S'", EAxisType::AxisY, 0, false, 1);
	SignInfos.Emplace("S2", EAxisType::AxisY, 0, true, 2);
	
	SignInfos.Emplace("F", EAxisType::AxisY, 1, true, 1);
	SignInfos.Emplace("F'", EAxisType::AxisY, 1, false, 1);
	SignInfos.Emplace("F2", EAxisType::AxisY, 1, true, 2);

	SignInfos.Emplace("D", EAxisType::AxisZ, -1, true, 1);
	SignInfos.Emplace("D'", EAxisType::AxisZ, -1, false, 1);
	SignInfos.Emplace("D2", EAxisType::AxisZ, -1, true, 2);
	
	SignInfos.Emplace("E", EAxisType::AxisZ, 0, false, 1);
	SignInfos.Emplace("E'", EAxisType::AxisZ, 0, true, 1);
	SignInfos.Emplace("E2", EAxisType::AxisZ, 0, false, 2);
	
	SignInfos.Emplace("U", EAxisType::AxisZ, 1, false, 1);
	SignInfos.Emplace("U'", EAxisType::AxisZ, 1, true, 1);
	SignInfos.Emplace("U2", EAxisType::AxisZ, 1, false, 2);
}

// Called when the game starts or when spawned
void ARCN_RubikCube::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TestTimerHandle;
	GetWorldTimerManager().SetTimer(TestTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		Spin("B D B2 R' B U2 R U' B2 F2 D F' U' L' B U' F2 U2 R2 L2 D' L R F2 L2");
	}), 1.0f, false);
}

// Called every frame
void ARCN_RubikCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARCN_RubikCube::Spin(const FString& Order)
{
	TArray<FString> ParsedOrders;
	Order.ParseIntoArray(ParsedOrders, TEXT(" "), true);

	for (const FString& ParsedOrder : ParsedOrders)
	{
		for (auto SignInfo : SignInfos)
		{
			if (ParsedOrder == SignInfo.Sign)
			{
				SignQueue.Emplace(SignInfo);
			}
		}
	}

	if (!bIsTurning)
	{
		bIsTurning = true;
		TurnNext();
	}
}

void ARCN_RubikCube::TurnNext()
{
	if (SignQueue.Num() == 0)
	{
		bIsTurning = false;
		return;
	}

	const FSignInfo NextSign = SignQueue[0];
	SignQueue.RemoveAt(0);

	TurnCore(NextSign);
}

void ARCN_RubikCube::TurnCore(const FSignInfo& SignInfo)
{
	GrabPieces(SignInfo);

	const float TargetAngle = SignInfo.TurnCount == 2 ? 180.0f : SignInfo.CCW ? 270.0f : 90.0f;
	
	FQuat TargetRotator = FQuat::Identity;
	switch (SignInfo.AxisType)
	{
	case EAxisType::AxisX:
		TargetRotator = FRotator(0.0f, 0.0f, TargetAngle).Quaternion();
		break;
        
	case EAxisType::AxisY:
		TargetRotator = FRotator(TargetAngle, 0.0f, 0.0f).Quaternion();
		break;
        
	case EAxisType::AxisZ:
		TargetRotator = FRotator(0.0f, TargetAngle, 0.0f).Quaternion();
		break;
	}
	
	UpdateTurnCore(SignInfo, TargetRotator);
}

void ARCN_RubikCube::UpdateTurnCore(const FSignInfo& SignInfo, FQuat TargetRotator)
{
	const FQuat CurrentRotator = CoreComponent->GetRelativeRotation().Quaternion();
	const FQuat NewRotator = FQuat::Slerp(CurrentRotator, TargetRotator, RubikCubeDataAsset->TurnSpeed);
	CoreComponent->SetRelativeRotation(NewRotator);

	if (NewRotator.Equals(TargetRotator, RubikCubeDataAsset->TurnTolerance))
	{
		CoreComponent->SetRelativeRotation(TargetRotator);
		ReleasePieces(SignInfo);
		return;
	}

	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this, SignInfo, TargetRotator]
	{
		UpdateTurnCore(SignInfo, TargetRotator);
	});
	GetWorldTimerManager().SetTimerForNextTick(TimerDelegate);
}

void ARCN_RubikCube::GrabPieces(const FSignInfo& SignInfo)
{
	for (const auto PieceMeshComponent : PieceMeshComponents)
	{
		switch (SignInfo.AxisType)
		{
		case EAxisType::AxisX:
			if (PieceMeshComponent.Value.X == SignInfo.Layer)
			{
				PieceMeshComponent.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		
		case EAxisType::AxisY:
			if (PieceMeshComponent.Value.Y == SignInfo.Layer)
			{
				PieceMeshComponent.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		
		case EAxisType::AxisZ:
			if (PieceMeshComponent.Value.Z == SignInfo.Layer)
			{
				PieceMeshComponent.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		}
	}
}

void ARCN_RubikCube::ReleasePieces(const FSignInfo& SignInfo)
{
	TArray<TObjectPtr<USceneComponent>> ChildComponents = CoreComponent->GetAttachChildren();
	for (const auto ChildComponent : ChildComponents)
	{
		if (UStaticMeshComponent* PieceMeshComponent = Cast<UStaticMeshComponent>(ChildComponent))
		{
			PieceMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

			FMatrix RotationMatrix;
			const int32 Sin = SignInfo.TurnCount == 2 ? 0 : SignInfo.CCW ? -1 : 1;
			const int32 Cos = SignInfo.TurnCount == 2 ? -1 : 0;
			
			switch (SignInfo.AxisType)
			{
			case EAxisType::AxisX:
				RotationMatrix = FMatrix(
					FVector(1, 0, 0),
					FVector(0, Cos, -Sin),
					FVector(0, Sin, Cos),
					FVector::ZeroVector
				);
				break;

			case EAxisType::AxisY:
				RotationMatrix = FMatrix(
					FVector(Cos, 0, Sin),
					FVector(0, 1, 0),
					FVector(-Sin, 0, Cos),
					FVector::ZeroVector
				);
				break;

			case EAxisType::AxisZ:
				RotationMatrix = FMatrix(
					FVector(Cos, -Sin, 0),
					FVector(Sin, Cos, 0),
					FVector(0, 0, 1),
					FVector::ZeroVector
				);
				break;
			}

			const FVector CurrentPosition = PieceMeshComponents[PieceMeshComponent];
			UE_LOG(LogTemp, Log, TEXT("C : %f, %f, %f"), CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z)
			const FVector NewPosition = RotationMatrix.TransformPosition(CurrentPosition);
			UE_LOG(LogTemp, Log, TEXT("N : %f, %f, %f"), NewPosition.X, NewPosition.Y, NewPosition.Z)
			UE_LOG(LogTemp, Log, TEXT("==========================="))
			PieceMeshComponents[PieceMeshComponent] = NewPosition;
		}
	}

	CoreComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
	TurnNext();
}

