// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_RubikCube.h"

#include "Data/RCN_RubikCubeDataAsset.h"
#include "KociembaAlgorithm/search.h"
#include "Util/EnumHelper.h"

DEFINE_LOG_CATEGORY(RubikCube);

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
		UE_LOG(RubikCube, Error, TEXT("데이터 에셋 로드 실패"))
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
					StickerPositions.Emplace(StickerMeshComponent, FVector(X - 1, Y, Z));
					StickerFacelets.Emplace(StickerMeshComponent, TEXT("L"));
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
					StickerPositions.Emplace(StickerMeshComponent, FVector(X + 1, Y, Z));
					StickerFacelets.Emplace(StickerMeshComponent, TEXT("R"));
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
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y - 1, Z));
					StickerFacelets.Emplace(StickerMeshComponent, TEXT("B"));
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
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y + 1, Z));
					StickerFacelets.Emplace(StickerMeshComponent, TEXT("F"));
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
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y, Z - 1));
					StickerFacelets.Emplace(StickerMeshComponent, TEXT("D"));
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
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y, Z + 1));
					StickerFacelets.Emplace(StickerMeshComponent, TEXT("U"));
				}

				PieceMeshComponents.Emplace(PieceMeshComponent);
				PieceMeshPositions.Emplace(PieceMeshComponent, FVector(X, Y, Z));
			}
		}
	}
	
	SignInfos.Emplace("L", EAxisType::AxisX, -1, false, 1);
	SignInfos.Emplace("L'", EAxisType::AxisX, -1, true, 1);
	SignInfos.Emplace("L2", EAxisType::AxisX, -1, false, 2);
	
	SignInfos.Emplace("M", EAxisType::AxisX, 0, false, 1);
	SignInfos.Emplace("M'", EAxisType::AxisX, 0, true, 1);
	SignInfos.Emplace("M2", EAxisType::AxisX, 0, false, 2);
	
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

	SignInfos.Emplace("D", EAxisType::AxisZ, -1, false, 1);
	SignInfos.Emplace("D'", EAxisType::AxisZ, -1, true, 1);
	SignInfos.Emplace("D2", EAxisType::AxisZ, -1, false, 2);
	
	SignInfos.Emplace("E", EAxisType::AxisZ, 0, false, 1);
	SignInfos.Emplace("E'", EAxisType::AxisZ, 0, true, 1);
	SignInfos.Emplace("E2", EAxisType::AxisZ, 0, false, 2);
	
	SignInfos.Emplace("U", EAxisType::AxisZ, 1, true, 1);
	SignInfos.Emplace("U'", EAxisType::AxisZ, 1, false, 1);
	SignInfos.Emplace("U2", EAxisType::AxisZ, 1, true, 2);

	for (int32 Y = -1; Y <= 1; Y++)
	{
		for (int32 X = -1; X <= 1; X++)
		{
			FaceletOrderPositions.Emplace(FVector(X, Y, 2));
		}
	}

	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 Y = 1; Y >= -1; Y--)
		{
			FaceletOrderPositions.Emplace(FVector(2, Y, Z));
		}
	}
	
	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 X = -1; X <= 1; X++)
		{
			FaceletOrderPositions.Emplace(FVector(X, 2, Z));
		}
	}

	for (int32 Y = 1; Y >= -1; Y--)
	{
		for (int32 X = -1; X <= 1; X++)
		{
			FaceletOrderPositions.Emplace(FVector(X, Y, -2));
		}
	}

	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 Y = -1; Y <= 1; Y++)
		{
			FaceletOrderPositions.Emplace(FVector(-2, Y, Z));
		}
	}

	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 X = 1; X >= -1; X--)
		{
			FaceletOrderPositions.Emplace(FVector(X, -2, Z));
		}
	}
}

// Called when the game starts or when spawned
void ARCN_RubikCube::BeginPlay()
{
	Super::BeginPlay();

	SortFacelet();

	FTimerHandle TestTimerHandle;
	GetWorldTimerManager().SetTimer(TestTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		Scramble();

		FTimerHandle TestTimerHandle2;
		GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateWeakLambda(this, [=, this]
		{
			FString SolveCommand = solution(
				TCHAR_TO_ANSI(*Facelet),
				30,
				1000,
				0,
				"cache"
			);

			Spin(SolveCommand);
		}), 6.0f, false);
	}), 12.0f, true);
}

// Called every frame
void ARCN_RubikCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARCN_RubikCube::Spin(const FString& Command)
{
	UE_LOG(RubikCube, Log, TEXT("큐브 명령어 입력 : %s"), *Command)
	
	TArray<FString> ParsedCommands;
	Command.ParseIntoArray(ParsedCommands, TEXT(" "), true);

	for (const FString& ParsedCommand : ParsedCommands)
	{
		for (auto SignInfo : SignInfos)
		{
			if (ParsedCommand == SignInfo.Sign)
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

void ARCN_RubikCube::Scramble()
{
	FString Command = TEXT("");

	for (int32 i = 0; i < RubikCubeDataAsset->ScrambleTurnCount; i++)
	{
		Command += SignInfos[FMath::RandRange(0, SignInfos.Num() - 1)].Sign + TEXT(" ");
	}

	Command.RemoveAt(Command.Len() - 1);
	Spin(Command);
}

void ARCN_RubikCube::TurnNext()
{
	if (SignQueue.Num() == 0)
	{
		bIsTurning = false;
		UE_LOG(RubikCube, Log, TEXT("회전 완료 및 패슬릿 : %s"), *Facelet)
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
		TargetRotator = FRotator(0.0f, -TargetAngle, 0.0f).Quaternion();
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

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, SignInfo, TargetRotator]
	{
		UpdateTurnCore(SignInfo, TargetRotator);
	}));
}

void ARCN_RubikCube::GrabPieces(const FSignInfo& SignInfo)
{
	for (const auto PieceMeshPosition : PieceMeshPositions)
	{
		switch (SignInfo.AxisType)
		{
		case EAxisType::AxisX:
			if (PieceMeshPosition.Value.X == SignInfo.Layer)
			{
				PieceMeshPosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		
		case EAxisType::AxisY:
			if (PieceMeshPosition.Value.Y == SignInfo.Layer)
			{
				PieceMeshPosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		
		case EAxisType::AxisZ:
			if (PieceMeshPosition.Value.Z == SignInfo.Layer)
			{
				PieceMeshPosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		}
	}
}

void ARCN_RubikCube::ReleasePieces(const FSignInfo& SignInfo)
{
	TArray<TObjectPtr<USceneComponent>> ChildPieceComponents = CoreComponent->GetAttachChildren();
	for (const auto ChildPieceComponent : ChildPieceComponents)
	{
		if (UStaticMeshComponent* PieceMeshComponent = Cast<UStaticMeshComponent>(ChildPieceComponent))
		{
			PieceMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

			const FVector CurrentPiecePosition = PieceMeshPositions[PieceMeshComponent];
			const FVector NewPiecePosition = GetRotationMatrix(SignInfo).TransformPosition(CurrentPiecePosition);
			PieceMeshPositions[PieceMeshComponent] = NewPiecePosition;

			TArray<TObjectPtr<USceneComponent>> ChildStickerComponents = PieceMeshComponent->GetAttachChildren();
			for (const auto ChildStickerComponent : ChildStickerComponents)
			{
				if (UStaticMeshComponent* StickerMeshComponent = Cast<UStaticMeshComponent>(ChildStickerComponent))
				{
					const FVector CurrentFaceletPosition = StickerPositions[StickerMeshComponent];
					const FVector NewFaceletPosition = GetRotationMatrix(SignInfo).TransformPosition(CurrentFaceletPosition);
					StickerPositions[StickerMeshComponent] = NewFaceletPosition;
				}
			}
		}
	}
	
	SortFacelet();

	CoreComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
	TurnNext();
}

FMatrix ARCN_RubikCube::GetRotationMatrix(const FSignInfo& SignInfo)
{
	const int32 Sin = SignInfo.TurnCount == 2 ? 0 : SignInfo.CCW ? -1 : 1;
	const int32 Cos = SignInfo.TurnCount == 2 ? -1 : 0;
			
	switch (SignInfo.AxisType)
	{
	case EAxisType::AxisX:
		return FMatrix(
			FVector(1, 0, 0),
			FVector(0, Cos, -Sin),
			FVector(0, Sin, Cos),
			FVector::ZeroVector
		);

	case EAxisType::AxisY:
		return FMatrix(
			FVector(Cos, 0, Sin),
			FVector(0, 1, 0),
			FVector(-Sin, 0, Cos),
			FVector::ZeroVector
		);

	case EAxisType::AxisZ:
		return FMatrix(
			FVector(Cos, -Sin, 0),
			FVector(Sin, Cos, 0),
			FVector(0, 0, 1),
			FVector::ZeroVector
		);
	}
	
	return FMatrix::Identity;
}

void ARCN_RubikCube::SortFacelet()
{
	Facelet = TEXT("");
	
	for (auto FaceletOrderPosition : FaceletOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (FaceletOrderPosition == StickerPosition.Value)
			{
				Facelet += StickerFacelets[StickerPosition.Key];
				break;
			}
		}
	}
}

