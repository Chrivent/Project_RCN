// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_RubikCube.h"

#include "Components/BoxComponent.h"
#include "Data/RCN_RubikCubeDataAsset.h"
#include "KociembaAlgorithm/CubeSolver.h"
#include "Project_RCN/Project_RCN.h"

const TArray<FSignInfo> ARCN_RubikCube::SignInfos = {
	{"L", EAxisType::AxisX, -1, false, 1}, {"L'", EAxisType::AxisX, -1, true, 1}, {"L2", EAxisType::AxisX, -1, false, 2},
	{"M", EAxisType::AxisX, 0, false, 1},  {"M'", EAxisType::AxisX, 0, true, 1},  {"M2", EAxisType::AxisX, 0, false, 2},
	{"R", EAxisType::AxisX, 1, true, 1},   {"R'", EAxisType::AxisX, 1, false, 1}, {"R2", EAxisType::AxisX, 1, true, 2},

	{"B", EAxisType::AxisY, -1, false, 1}, {"B'", EAxisType::AxisY, -1, true, 1}, {"B2", EAxisType::AxisY, -1, false, 2},
	{"S", EAxisType::AxisY, 0, true, 1},   {"S'", EAxisType::AxisY, 0, false, 1}, {"S2", EAxisType::AxisY, 0, true, 2},
	{"F", EAxisType::AxisY, 1, true, 1},   {"F'", EAxisType::AxisY, 1, false, 1}, {"F2", EAxisType::AxisY, 1, true, 2},

	{"D", EAxisType::AxisZ, -1, false, 1}, {"D'", EAxisType::AxisZ, -1, true, 1}, {"D2", EAxisType::AxisZ, -1, false, 2},
	{"E", EAxisType::AxisZ, 0, false, 1},  {"E'", EAxisType::AxisZ, 0, true, 1},  {"E2", EAxisType::AxisZ, 0, false, 2},
	{"U", EAxisType::AxisZ, 1, true, 1},   {"U'", EAxisType::AxisZ, 1, false, 1}, {"U2", EAxisType::AxisZ, 1, true, 2}
};

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

				UStaticMeshComponent* PieceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("PieceComponent %d"), PieceMeshComponents.Num()));
				
				PieceMeshComponent->SetupAttachment(RootComponent);
				PieceMeshComponent->SetRelativeLocation(FVector(X * PieceDistance, Y * PieceDistance, Z * PieceDistance));
				PieceMeshComponent->SetRelativeScale3D(FVector(PieceSize));
				PieceMeshComponent->SetStaticMesh(RubikCubeDataAsset->PieceMesh);
				PieceMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				if (X == -1)
				{
					CreateStickerAndButton(PieceMeshComponent, PieceSize, FVector(X - 1, Y, Z), EStickerType::Orange);
				}

				if (X == 1)
				{
					CreateStickerAndButton(PieceMeshComponent, PieceSize, FVector(X + 1, Y, Z), EStickerType::Red);
				}

				if (Y == -1)
				{
					CreateStickerAndButton(PieceMeshComponent, PieceSize, FVector(X, Y - 1, Z), EStickerType::Green);
				}

				if (Y == 1)
				{
					CreateStickerAndButton(PieceMeshComponent, PieceSize, FVector(X, Y + 1, Z), EStickerType::Blue);
				}

				if (Z == -1)
				{
					CreateStickerAndButton(PieceMeshComponent, PieceSize, FVector(X, Y, Z - 1), EStickerType::White);
				}

				if (Z == 1)
				{
					CreateStickerAndButton(PieceMeshComponent, PieceSize, FVector(X, Y, Z + 1), EStickerType::Yellow);
				}

				PieceMeshComponents.Emplace(PieceMeshComponent);
				PiecePositions.Emplace(PieceMeshComponent, FVector(X, Y, Z));
			}
		}
	}

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

	bReplicates = true;
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

void ARCN_RubikCube::Spin(const FString& Command)
{
	RCN_LOG(LogRubikCube, Log, TEXT("큐브 명령어 입력 : %s"), *Command)
	
	ServerRPC_Spin(Command);
}

void ARCN_RubikCube::Scramble()
{
	RCN_LOG(LogRubikCube, Log, TEXT("큐브 섞기"))
	
	ServerRPC_Scramble();
}

void ARCN_RubikCube::Solve()
{
	RCN_LOG(LogRubikCube, Log, TEXT("큐브 풀기"))
	
	ServerRPC_Solve();
}

void ARCN_RubikCube::ChangePattern(const FString& NewPattern)
{
	MulticastRPC_ChangePattern(NewPattern);
}

FVector ARCN_RubikCube::GetButtonPosition(UBoxComponent* ButtonBoxComponent)
{
	for (const auto ButtonPosition : ButtonPositions)
	{
		if (ButtonPosition.Key == ButtonBoxComponent)
		{
			return ButtonPosition.Value;
		}
	}

	return FVector::ZeroVector;
}

void ARCN_RubikCube::CreateStickerAndButton(UStaticMeshComponent* PieceMeshComponent, const float PieceSize, const FVector& Position, const EStickerType StickerType)
{
	const float StickerDistance = RubikCubeDataAsset->StickerDistance;
	const float StickerSize = RubikCubeDataAsset->StickerSize;

	const float ButtonThickness = RubikCubeDataAsset->ButtonThickness;
	const float ButtonSize = RubikCubeDataAsset->ButtonSize;
	
	UStaticMeshComponent* StickerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("StickerComponent %d"), StickerMeshComponents.Num()));
					
	StickerMeshComponent->SetupAttachment(PieceMeshComponent);
	if (FMath::Abs(Position.X) == 2)
	{
		StickerMeshComponent->SetRelativeLocation(FVector(StickerDistance * Position.X / 2, 0, 0));
		StickerMeshComponent->SetRelativeRotation(FRotator(-90.0f * Position.X / 2, 0, 0));
	}
	else if (FMath::Abs(Position.Y) == 2)
	{
		StickerMeshComponent->SetRelativeLocation(FVector(0, StickerDistance * Position.Y / 2, 0));
		StickerMeshComponent->SetRelativeRotation(FRotator(0, 0, 90.0f * Position.Y / 2));
	}
	else if (FMath::Abs(Position.Z) == 2)
	{
		StickerMeshComponent->SetRelativeLocation(FVector(0, 0, StickerDistance * Position.Z / 2));
		StickerMeshComponent->SetRelativeRotation(FRotator(-180.0f * FMath::Clamp(Position.Z, -1, 0), 0, 0));
	}
	StickerMeshComponent->SetRelativeScale3D(FVector(StickerSize));
	StickerMeshComponent->SetStaticMesh(RubikCubeDataAsset->StickerMesh);
	StickerMeshComponent->SetMaterial(0, RubikCubeDataAsset->StickerMaterials[StickerType]);
	StickerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StickerMeshComponents.Emplace(StickerMeshComponent);
	StickerPositions.Emplace(StickerMeshComponent, Position);

	UBoxComponent* ButtonBoxComponent = CreateDefaultSubobject<UBoxComponent>(*FString::Printf(TEXT("Button %d"), ButtonBoxComponents.Num()));

	ButtonBoxComponent->SetupAttachment(RootComponent);
	ButtonBoxComponent->SetRelativeLocation(PieceMeshComponent->GetRelativeLocation() + StickerMeshComponent->GetRelativeLocation() * PieceSize);
	if (FMath::Abs(Position.X) == 2)
	{
		ButtonBoxComponent->SetBoxExtent(FVector(ButtonThickness, ButtonSize, ButtonSize));
	}
	else if (FMath::Abs(Position.Y) == 2)
	{
		ButtonBoxComponent->SetBoxExtent(FVector(ButtonSize, ButtonThickness, ButtonSize));
	}
	else if (FMath::Abs(Position.Z) == 2)
	{
		ButtonBoxComponent->SetBoxExtent(FVector(ButtonSize, ButtonSize, ButtonThickness));
	}
	ButtonBoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ButtonBoxComponent->SetHiddenInGame(false);

	ButtonBoxComponents.Emplace(ButtonBoxComponent);
	ButtonPositions.Emplace(ButtonBoxComponent, Position);
}

void ARCN_RubikCube::TurnNext()
{
	if (SignQueue.IsEmpty())
	{
		RCN_LOG(LogRubikCube, Log, TEXT("회전 완료 및 패턴 : %s"), *Pattern)
		bIsTurning = false;

		if (HasAuthority())
		{
			if (bIsScrambling)
			{
				RCN_LOG(LogRubikCube, Log, TEXT("섞기 완료"))
				bIsScrambling = false;
				FinishScrambleDelegate.Broadcast();
			}

			TArray FacesPatterns = {
				Pattern.Mid(0, 9),
				Pattern.Mid(9, 9),
				Pattern.Mid(18, 9),
				Pattern.Mid(27, 9),
				Pattern.Mid(36, 9),
				Pattern.Mid(45, 9)
			};

			bool bIsSolved = true;
			for (auto FacesPattern : FacesPatterns)
			{
				const TCHAR FirstStickerColor = FacesPattern[0];
				for (const auto StickerColor : FacesPattern)
				{
					if (StickerColor != FirstStickerColor)
					{
						bIsSolved = false;
					}
				}
			}

			if (bIsSolved)
			{
				RCN_LOG(LogRubikCube, Log, TEXT("풀기 완료"))
				FinishSolveDelegate.Broadcast();
			}
		}
		
		return;
	}

	FSignInfo NextSign;
	SignQueue.Dequeue(NextSign);

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

void ARCN_RubikCube::ServerRPC_Spin_Implementation(const FString& Command)
{
	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("Begin"));

	MulticastRPC_Spin(Command);
	MulticastRPC_ChangePattern(Pattern);

	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_RubikCube::MulticastRPC_Spin_Implementation(const FString& Command)
{
	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("Begin"));

	TArray<FString> ParsedCommands;
	Command.ParseIntoArray(ParsedCommands, TEXT(" "), true);

	for (const FString& ParsedCommand : ParsedCommands)
	{
		for (auto SignInfo : SignInfos)
		{
			if (ParsedCommand == SignInfo.Sign)
			{
				SignQueue.Enqueue(SignInfo);
			}
		}
	}

	if (!bIsTurning)
	{
		bIsTurning = true;

		TurnNext();
	}

	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_RubikCube::ServerRPC_Scramble_Implementation()
{
	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("Begin"));

	if (bIsTurning)
	{
		RCN_LOG(LogRubikCube, Warning, TEXT("%s"), TEXT("큐브가 회전 중입니다. 회전이 끝날 때까지 기다려주세요."));
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
	
	ServerRPC_Spin(Command);

	bIsScrambling = true;

	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_RubikCube::ServerRPC_Solve_Implementation()
{
	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("Begin"));

	if (bIsTurning)
	{
		RCN_LOG(LogRubikCube, Warning, TEXT("%s"), TEXT("큐브가 회전 중입니다. 회전이 끝날 때까지 기다려주세요."));
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

	Command = UCubeSolver::SolveCube(
		Facelet,
		24,
		1000,
		0,
		"cache"
	);

	ServerRPC_Spin(Command);

	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_RubikCube::MulticastRPC_ChangePattern_Implementation(const FString& NewPattern)
{
	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("Begin"));

	if (Pattern != NewPattern)
	{
		RCN_LOG(LogRubikCube, Warning, TEXT("%s"), TEXT("패턴 보정"));
		
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

	RCN_LOG(LogRubikCube, Log, TEXT("%s"), TEXT("End"));
}
