// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_RubikCube.h"

#include "Data/RCN_RubikCubeDataAsset.h"
#include "KociembaAlgorithm/search.h"
#include "Project_RCN/Project_RCN.h"
#include "Util/EnumHelper.h"

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
		RCN_LOG(RubikCube, Error, TEXT("데이터 에셋 로드 실패"))
		return;
	}

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;

	PitchComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PitchComponent"));
	PitchComponent->SetupAttachment(RootComponent);
	PitchComponent->SetRelativeRotation(FRotator(30.0f, 0.0f, 0.0f));

	YawComponent = CreateDefaultSubobject<USceneComponent>(TEXT("YawComponent"));
	YawComponent->SetupAttachment(PitchComponent);
	YawComponent->SetRelativeRotation(FRotator(0.0f, 120.0f, 0.0f));

	CoreComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CoreComponent"));
	CoreComponent->SetupAttachment(YawComponent);

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
				
				PieceMeshComponent->SetupAttachment(YawComponent);
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
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Orange]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X - 1, Y, Z));
					StickerColors.Emplace(StickerMeshComponent, FColor::Orange);
				}

				if (X == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Red [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(StickerDistance, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Red]);
					
					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X + 1, Y, Z));
					StickerColors.Emplace(StickerMeshComponent, FColor::Red);
				}

				if (Y == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Green [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, -StickerDistance, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Green]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y - 1, Z));
					StickerColors.Emplace(StickerMeshComponent, FColor::Green);
				}

				if (Y == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Blue [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, StickerDistance, 0.0f));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Blue]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y + 1, Z));
					StickerColors.Emplace(StickerMeshComponent, FColor::Blue);
				}

				if (Z == -1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent White [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -StickerDistance));
					StickerMeshComponent->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::White]);
					
					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y, Z - 1));
					StickerColors.Emplace(StickerMeshComponent, FColor::White);
				}

				if (Z == 1)
				{
					UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent Yellow [%d, %d, %d]"), X, Y, Z));
					
					StickerMeshComponent->SetupAttachment(PieceMeshComponent);
					StickerMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, StickerDistance));
					StickerMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
					StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
					StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh[EStickerType::Yellow]);

					StickerMeshComponents.Emplace(StickerMeshComponent);
					StickerPositions.Emplace(StickerMeshComponent, FVector(X, Y, Z + 1));
					StickerColors.Emplace(StickerMeshComponent, FColor::Yellow);
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

	CenterOrderPositions.Emplace(FVector(0, 0, 2));
	CenterOrderPositions.Emplace(FVector(2, 0, 0));
	CenterOrderPositions.Emplace(FVector(0, 2, 0));
	CenterOrderPositions.Emplace(FVector(0, 0, -2));
	CenterOrderPositions.Emplace(FVector(-2, 0, 0));
	CenterOrderPositions.Emplace(FVector(0, -2, 0));

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

	/*FTimerHandle TestTimerHandle;
	GetWorldTimerManager().SetTimer(TestTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		Scramble();

		FTimerHandle TestTimerHandle2;
		GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateWeakLambda(this, [=, this]
		{
			Solve();
		}), 3.0f, false);
	}), 6.0f, true);*/
}

// Called every frame
void ARCN_RubikCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ARCN_RubikCube::Scramble()
{
	Spin(GetScrambleCommand());
}

void ARCN_RubikCube::Solve()
{
	bRequestedSolving = true;

	if (!bIsTurning)
	{
		TurnNext();
	}
}

FString ARCN_RubikCube::GetScrambleCommand()
{
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

	return Command;
}

FString ARCN_RubikCube::GetSolveCommand()
{
	return solution(
		TCHAR_TO_ANSI(*Facelet),
		24,
		1000,
		0,
		"cache"
	);
}

void ARCN_RubikCube::Spin(const FString& Command)
{
	RCN_LOG(RubikCube, Log, TEXT("큐브 명령어 입력 : %s"), *Command)

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

void ARCN_RubikCube::Rotate(FVector2D RotateAxisVector)
{
	FRotator PitchRotator = PitchComponent->GetRelativeRotation();
	FRotator YawRotator = YawComponent->GetRelativeRotation();

	PitchRotator.Pitch = FMath::Clamp(PitchComponent->GetRelativeRotation().Pitch + RotateAxisVector.Y, -89.0f, 89.0f);
	YawRotator.Yaw = YawComponent->GetRelativeRotation().Yaw + RotateAxisVector.X;

	PitchComponent->SetRelativeRotation(PitchRotator);
	YawComponent->SetRelativeRotation(YawRotator);
}

void ARCN_RubikCube::TurnNext()
{
	if (bRequestedSolving)
	{
		bRequestedSolving = false;
		
		bIsTurning = false;
		SignQueue.Empty();
		
		const FString SolveCommand = GetSolveCommand();
		Spin(SolveCommand);
		
		return;
	}
	
	if (SignQueue.Num() == 0)
	{
		bIsTurning = false;
		RCN_LOG(RubikCube, Log, TEXT("회전 완료 및 패슬릿 : %s"), *Facelet)
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

void ARCN_RubikCube::UpdateTurnCore(const FSignInfo& SignInfo, FQuat TargetQuat)
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

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, SignInfo, TargetQuat]
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
			PieceMeshComponent->AttachToComponent(YawComponent, FAttachmentTransformRules::KeepWorldTransform);

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
	
	SortFacelet();

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

void ARCN_RubikCube::SortFacelet()
{
	Facelet = TEXT("");
	
	TArray<FColor> CenterOrderColors;
	for (auto CenterOrderPosition : CenterOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (CenterOrderPosition == StickerPosition.Value)
			{
				CenterOrderColors.Emplace(StickerColors[StickerPosition.Key]);
			}
		}
	}
	
	for (auto FaceletOrderPosition : FaceletOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (FaceletOrderPosition == StickerPosition.Value)
			{
				if (CenterOrderColors[0] == StickerColors[StickerPosition.Key])
				{
					Facelet += 'U';
				}
				else if (CenterOrderColors[1] == StickerColors[StickerPosition.Key])
				{
					Facelet += 'R';
				}
				else if (CenterOrderColors[2] == StickerColors[StickerPosition.Key])
				{
					Facelet += 'F';
				}
				else if (CenterOrderColors[3] == StickerColors[StickerPosition.Key])
				{
					Facelet += 'D';
				}
				else if (CenterOrderColors[4] == StickerColors[StickerPosition.Key])
				{
					Facelet += 'L';
				}
				else if (CenterOrderColors[5] == StickerColors[StickerPosition.Key])
				{
					Facelet += 'B';
				}
				
				break;
			}
		}
	}
}

