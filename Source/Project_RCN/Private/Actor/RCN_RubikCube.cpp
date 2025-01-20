// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_RubikCube.h"

#include "Data/RCN_RubikCubeDataAsset.h"
#include "KociembaAlgorithm/search.h"
#include "Project_RCN/Project_RCN.h"

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
		RCN_LOG(LogRubikCube, Error, TEXT("데이터 에셋 로드 실패"))
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
					StickerMeshComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
					StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Orange]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X - 1, Y, Z));
				}

				if (X == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Red [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(StickerDistance, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
					StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Red]);
					
					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X + 1, Y, Z));
				}

				if (Y == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Green [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, -StickerDistance, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
					StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Green]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y - 1, Z));
				}

				if (Y == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Blue [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, StickerDistance, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
					StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Blue]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y + 1, Z));
				}

				if (Z == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent White [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -StickerDistance));
					StickerMeshComponent->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
					StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::White]);
					
					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y, Z - 1));
				}

				if (Z == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Yellow [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, StickerDistance));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
					StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Yellow]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y, Z + 1));
				}

				PieceMeshComponents.Emplace(PieceMeshComponent);
				PiecePositions.Emplace(PieceMeshComponent, FVector(X, Y, Z));
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
			PatternOrderPositions.Emplace(FVector(X, Y, 2));
		}
	}

	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 Y = 1; Y >= -1; Y--)
		{
			PatternOrderPositions.Emplace(FVector(2, Y, Z));
		}
	}
	
	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 X = -1; X <= 1; X++)
		{
			PatternOrderPositions.Emplace(FVector(X, 2, Z));
		}
	}

	for (int32 Y = 1; Y >= -1; Y--)
	{
		for (int32 X = -1; X <= 1; X++)
		{
			PatternOrderPositions.Emplace(FVector(X, Y, -2));
		}
	}

	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 Y = -1; Y <= 1; Y++)
		{
			PatternOrderPositions.Emplace(FVector(-2, Y, Z));
		}
	}

	for (int32 Z = 1; Z >= -1; Z--)
	{
		for (int32 X = 1; X >= -1; X--)
		{
			PatternOrderPositions.Emplace(FVector(X, -2, Z));
		}
	}

	Pattern = TEXT("YYYYYYYYYRRRRRRRRRBBBBBBBBBWWWWWWWWWOOOOOOOOOGGGGGGGGG");
}

// Called when the game starts or when spawned
void ARCN_RubikCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARCN_RubikCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ARCN_RubikCube::Scramble()
{
	if (bIsTurning)
	{
		return;
	}
	
	FString Command = TEXT("");

	FString LastSign = TEXT(" ");
	for (int32 i = 0; i < RubikCubeDataAsset->ScrambleTurnCount; i++)
	{
		FString CurrentSign;
		do
		{
			CurrentSign = SignInfos[FMath::RandRange(0, SignInfos.Num() - 1)].Sign;
		} while (LastSign[0] == CurrentSign[0]);
		LastSign = CurrentSign;

		Command += CurrentSign + TEXT(" ");
	}

	Command.RemoveAt(Command.Len() - 1);
	
	Spin(Command);

	bIsScrambling = true;
}

void ARCN_RubikCube::Solve()
{
	if (bIsTurning)
	{
		return;
	}
	
	FString Command = TEXT("");

	TMap<TCHAR, TCHAR> ReplacementInfo = {
		{ Pattern[4], TEXT('U') },
		{ Pattern[13], TEXT('R') },
		{ Pattern[22], TEXT('F') },
		{ Pattern[31], TEXT('D') },
		{ Pattern[40], TEXT('L') },
		{ Pattern[49], TEXT('B') }
	};

	FString Facelet = Pattern;
	for (auto& FaceletChar : Facelet)
	{
		FaceletChar = ReplacementInfo[FaceletChar];
	}

	Command = solution(
		TCHAR_TO_ANSI(*Facelet),
		24,
		1000,
		0,
		"cache"
	);

	Spin(Command);
}

void ARCN_RubikCube::ChangePattern(const FString& NewPattern)
{
	if (bIsTurning)
	{
		GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
		{
			ChangePattern(NewPattern);
		}));
	}
	
	Pattern = NewPattern;
	
	int32 Index = 0;
	for (auto PatternOrderPosition : PatternOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (PatternOrderPosition == StickerPosition.Value)
			{
				if (NewPattern[Index] == TEXT('Y'))
				{
					StickerPosition.Key->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Yellow]);
				}
				else if (NewPattern[Index] == TEXT('R'))
				{
					StickerPosition.Key->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Red]);
				}
				else if (NewPattern[Index] == TEXT('B'))
				{
					StickerPosition.Key->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Blue]);
				}
				else if (NewPattern[Index] == TEXT('W'))
				{
					StickerPosition.Key->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::White]);
				}
				else if (NewPattern[Index] == TEXT('O'))
				{
					StickerPosition.Key->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Orange]);
				}
				else if (NewPattern[Index] == TEXT('G'))
				{
					StickerPosition.Key->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[EStickerType::Green]);
				}

				Index++;
				break;
			}
		}
	}
}

FVector ARCN_RubikCube::GetStickerPosition(UStaticMeshComponent* StickerMeshComponent)
{
	for (const auto StickerPosition : StickerPositions)
	{
		if (StickerPosition.Key == StickerMeshComponent)
		{
			return StickerPosition.Value;
		}
	}

	return FVector::ZeroVector;
}

void ARCN_RubikCube::Spin(const FString& Command)
{
	RCN_LOG(LogRubikCube, Log, TEXT("큐브 명령어 입력 : %s"), *Command)

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

	SpinDelegate.Broadcast(Command);
}

void ARCN_RubikCube::TurnNext()
{
	if (SignQueue.Num() == 0)
	{
		bIsTurning = false;

		RCN_LOG(LogRubikCube, Log, TEXT("회전 완료 및 패턴 : %s"), *Pattern)
		PatternChangedDelegate.Broadcast(Pattern);

		if (bIsScrambling)
		{
			bIsScrambling = false;
			FinishScrambleDelegate.Broadcast();
		}
		
		return;
	}

	const FSignInfo NextSign = SignQueue[0];
	SignQueue.RemoveAt(0);

	TurnCore(NextSign);
}

void ARCN_RubikCube::TurnCore(const FSignInfo& SignInfo)
{
	GrabPieces(SignInfo);

	const float TargetAngle = SignInfo.TurnCount == 2 ? 180.0f : SignInfo.CCW ? -90.0f : 90.0f;
	
	FQuat TargetQuat = FQuat::Identity;
	switch (SignInfo.AxisType)
	{
	case EAxisType::AxisX:
		TargetQuat = FRotator(0.0f, 0.0f, TargetAngle).Quaternion();
		break;
        
	case EAxisType::AxisY:
		TargetQuat = FRotator(TargetAngle, 0.0f, 0.0f).Quaternion();
		break;
        
	case EAxisType::AxisZ:
		TargetQuat = FRotator(0.0f, -TargetAngle, 0.0f).Quaternion();
		break;
	}
	
	UpdateTurnCore(SignInfo, TargetQuat);
}

void ARCN_RubikCube::UpdateTurnCore(const FSignInfo& SignInfo, const FQuat& TargetQuat)
{
	const FQuat CurrentQuat = CoreComponent->GetRelativeRotation().Quaternion();
	const FQuat NewQuat = FQuat::Slerp(CurrentQuat, TargetQuat, RubikCubeDataAsset->TurnSpeed);
	CoreComponent->SetRelativeRotation(NewQuat);

	if (NewQuat.Equals(TargetQuat, RubikCubeDataAsset->TurnTolerance))
	{
		CoreComponent->SetRelativeRotation(TargetQuat);
		ReleasePieces(SignInfo);
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateTurnCore(SignInfo, TargetQuat);
	}));
}

void ARCN_RubikCube::GrabPieces(const FSignInfo& SignInfo)
{
	for (const auto PiecePosition : PiecePositions)
	{
		switch (SignInfo.AxisType)
		{
		case EAxisType::AxisX:
			if (PiecePosition.Value.X == SignInfo.Layer)
			{
				PiecePosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepWorldTransform);
			}
			break;
		
		case EAxisType::AxisY:
			if (PiecePosition.Value.Y == SignInfo.Layer)
			{
				PiecePosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepWorldTransform);
			}
			break;
		
		case EAxisType::AxisZ:
			if (PiecePosition.Value.Z == SignInfo.Layer)
			{
				PiecePosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepWorldTransform);
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

			const FVector CurrentPiecePosition = PiecePositions[PieceMeshComponent];
			const FVector NewPiecePosition = GetRotationMatrix(SignInfo).TransformPosition(CurrentPiecePosition);
			PiecePositions[PieceMeshComponent] = NewPiecePosition;

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
	
	Pattern = TEXT("");
	
	for (auto PatternOrderPosition : PatternOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (PatternOrderPosition == StickerPosition.Value)
			{
				if (StickerPosition.Key->GetMaterial(0) == RubikCubeDataAsset->StickerMaterials[EStickerType::Yellow])
				{
					Pattern += 'Y';
				}
				else if (StickerPosition.Key->GetMaterial(0) == RubikCubeDataAsset->StickerMaterials[EStickerType::Red])
				{
					Pattern += 'R';
				}
				else if (StickerPosition.Key->GetMaterial(0) == RubikCubeDataAsset->StickerMaterials[EStickerType::Blue])
				{
					Pattern += 'B';
				}
				else if (StickerPosition.Key->GetMaterial(0) == RubikCubeDataAsset->StickerMaterials[EStickerType::White])
				{
					Pattern += 'W';
				}
				else if (StickerPosition.Key->GetMaterial(0) == RubikCubeDataAsset->StickerMaterials[EStickerType::Orange])
				{
					Pattern += 'O';
				}
				else if (StickerPosition.Key->GetMaterial(0) == RubikCubeDataAsset->StickerMaterials[EStickerType::Green])
				{
					Pattern += 'G';
				}
				
				break;
			}
		}
	}

	CoreComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
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

