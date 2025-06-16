#include "RubikCube.h"

#include "RubikCubeSolver.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogRubikCube);

const TArray<FSignInfo> ARubikCube::SignInfos = {
	{ "L", ECubeAxisType::X, -1, false, 1 }, { "L'", ECubeAxisType::X, -1,  true, 1 }, { "L2", ECubeAxisType::X, -1, false, 2 },
	{ "M", ECubeAxisType::X,  0, false, 1 }, { "M'", ECubeAxisType::X,  0,  true, 1 }, { "M2", ECubeAxisType::X,  0, false, 2 },
	{ "R", ECubeAxisType::X,  1,  true, 1 }, { "R'", ECubeAxisType::X,  1, false, 1 }, { "R2", ECubeAxisType::X,  1,  true, 2 },

	{ "B", ECubeAxisType::Y, -1, false, 1 }, { "B'", ECubeAxisType::Y, -1,  true, 1 }, { "B2", ECubeAxisType::Y, -1, false, 2 },
	{ "S", ECubeAxisType::Y,  0,  true, 1 }, { "S'", ECubeAxisType::Y,  0, false, 1 }, { "S2", ECubeAxisType::Y,  0,  true, 2 },
	{ "F", ECubeAxisType::Y,  1,  true, 1 }, { "F'", ECubeAxisType::Y,  1, false, 1 }, { "F2", ECubeAxisType::Y,  1,  true, 2 },

	{ "D", ECubeAxisType::Z, -1, false, 1 }, { "D'", ECubeAxisType::Z, -1,  true, 1 }, { "D2", ECubeAxisType::Z, -1, false, 2 },
	{ "E", ECubeAxisType::Z,  0, false, 1 }, { "E'", ECubeAxisType::Z,  0,  true, 1 }, { "E2", ECubeAxisType::Z,  0, false, 2 },
	{ "U", ECubeAxisType::Z,  1,  true, 1 }, { "U'", ECubeAxisType::Z,  1, false, 1 }, { "U2", ECubeAxisType::Z,  1,  true, 2 }
};

const TArray<FVector> ARubikCube::FaceletOrderPositions = {
	FVector(-1, -1,  2), FVector( 0, -1,  2), FVector( 1, -1,  2),
	FVector(-1,  0,  2), FVector( 0,  0,  2), FVector( 1,  0,  2),
	FVector(-1,  1,  2), FVector( 0,  1,  2), FVector( 1,  1,  2),

	FVector( 2,  1,  1), FVector( 2,  0,  1), FVector( 2, -1,  1),
	FVector( 2,  1,  0), FVector( 2,  0,  0), FVector( 2, -1,  0),
	FVector( 2,  1, -1), FVector( 2,  0, -1), FVector( 2, -1, -1),

	FVector(-1,  2,  1), FVector( 0,  2,  1), FVector( 1,  2,  1),
	FVector(-1,  2,  0), FVector( 0,  2,  0), FVector( 1,  2,  0),
	FVector(-1,  2, -1), FVector( 0,  2, -1), FVector( 1,  2, -1),

	FVector(-1,  1, -2), FVector( 0,  1, -2), FVector( 1,  1, -2),
	FVector(-1,  0, -2), FVector( 0,  0, -2), FVector( 1,  0, -2),
	FVector(-1, -1, -2), FVector( 0, -1, -2), FVector( 1, -1, -2),

	FVector(-2, -1,  1), FVector(-2,  0,  1), FVector(-2,  1,  1),
	FVector(-2, -1,  0), FVector(-2,  0,  0), FVector(-2,  1,  0),
	FVector(-2, -1, -1), FVector(-2,  0, -1), FVector(-2,  1, -1),

	FVector( 1, -2,  1), FVector( 0, -2,  1), FVector(-1, -2,  1),
	FVector( 1, -2,  0), FVector( 0, -2,  0), FVector(-1, -2,  0),
	FVector( 1, -2, -1), FVector( 0, -2, -1), FVector(-1, -2, -1),
};

ARubikCube::ARubikCube()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;

	CoreComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CoreComponent"));
	CoreComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> RedMat(TEXT("/Script/Engine.Material'/CubeSolver/M_ClassicSticker_Red.M_ClassicSticker_Red'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> OrangeMat(TEXT("/Script/Engine.Material'/CubeSolver/M_ClassicSticker_Orange.M_ClassicSticker_Orange'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> YellowMat(TEXT("/Script/Engine.Material'/CubeSolver/M_ClassicSticker_Yellow.M_ClassicSticker_Yellow'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> GreenMat(TEXT("/Script/Engine.Material'/CubeSolver/M_ClassicSticker_Green.M_ClassicSticker_Green'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueMat(TEXT("/Script/Engine.Material'/CubeSolver/M_ClassicSticker_Blue.M_ClassicSticker_Blue'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> WhiteMat(TEXT("/Script/Engine.Material'/CubeSolver/M_ClassicSticker_White.M_ClassicSticker_White'"));

	if (RedMat.Succeeded()) StickerMaterials.Add(EStickerType::Red, RedMat.Object);
	if (OrangeMat.Succeeded()) StickerMaterials.Add(EStickerType::Orange, OrangeMat.Object);
	if (YellowMat.Succeeded()) StickerMaterials.Add(EStickerType::Yellow, YellowMat.Object);
	if (GreenMat.Succeeded()) StickerMaterials.Add(EStickerType::Green, GreenMat.Object);
	if (BlueMat.Succeeded()) StickerMaterials.Add(EStickerType::Blue, BlueMat.Object);
	if (WhiteMat.Succeeded()) StickerMaterials.Add(EStickerType::White, WhiteMat.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PieceMeshRef(TEXT("/Script/Engine.StaticMesh'/CubeSolver/SM_ClassicPiece.SM_ClassicPiece'"));
	if (PieceMeshRef.Succeeded()) PieceMesh = PieceMeshRef.Object;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StickerMeshRef(TEXT("/Script/Engine.StaticMesh'/CubeSolver/SM_ClassicSticker.SM_ClassicSticker'"));
	if (StickerMeshRef.Succeeded()) StickerMesh = StickerMeshRef.Object;

	PieceDistance = 100.0f;
	PieceSize = 0.5f;
	TurnSpeed = 30.0f;
	StickerDistance = 102.0f;
	StickerSize = 0.8f;
	ButtonSize = 50.0f;
	ButtonThickness = 2.0f;

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
				PieceMeshComponent->SetStaticMesh(PieceMesh);
				PieceMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				if (X == -1)
				{
					CreateStickerAndButton(PieceMeshComponent, FVector(X - 1, Y, Z), EStickerType::Orange);
				}

				if (X == 1)
				{
					CreateStickerAndButton(PieceMeshComponent, FVector(X + 1, Y, Z), EStickerType::Red);
				}

				if (Y == -1)
				{
					CreateStickerAndButton(PieceMeshComponent, FVector(X, Y - 1, Z), EStickerType::Green);
				}

				if (Y == 1)
				{
					CreateStickerAndButton(PieceMeshComponent, FVector(X, Y + 1, Z), EStickerType::Blue);
				}

				if (Z == -1)
				{
					CreateStickerAndButton(PieceMeshComponent, FVector(X, Y, Z - 1), EStickerType::White);
				}

				if (Z == 1)
				{
					CreateStickerAndButton(PieceMeshComponent, FVector(X, Y, Z + 1), EStickerType::Yellow);
				}

				PieceMeshComponents.Emplace(PieceMeshComponent);
				PiecePositions.Emplace(PieceMeshComponent, FVector(X, Y, Z));
			}
		}
	}

	bIsSolved = true;
	Facelets = TEXT("YYYYYYYYYRRRRRRRRRBBBBBBBBBWWWWWWWWWOOOOOOOOOGGGGGGGGG");

	bReplicates = true;
}

void ARubikCube::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsUpdatingTurnCore)
	{
		const FQuat CurrentQuat = CoreComponent->GetRelativeRotation().Quaternion();
		const float Alpha = FMath::Clamp(TurnSpeed * DeltaSeconds, 0.0f, 1.0f);
		const FQuat NewQuat = FQuat::Slerp(CurrentQuat, CurrentTargetQuat, Alpha);

		CoreComponent->SetRelativeRotation(NewQuat);

		if (NewQuat.Equals(CurrentTargetQuat, 0.01f))
		{
			CoreComponent->SetRelativeRotation(CurrentTargetQuat);
			bIsUpdatingTurnCore = false;
			ReleasePieces(CurrentSignInfo);
		}
	}
}

void ARubikCube::Spin(const FString& Command)
{
	UE_LOG(LogRubikCube, Log, TEXT("큐브 명령어 입력 : %s"), *Command)
	
	ServerRPC_Spin(Command);
}

void ARubikCube::Scramble()
{
	UE_LOG(LogRubikCube, Log, TEXT("큐브 섞기"))
	
	ServerRPC_Scramble();
}

void ARubikCube::Solve()
{
	UE_LOG(LogRubikCube, Log, TEXT("큐브 풀기"))
	
	ServerRPC_Solve();
}

FVector ARubikCube::GetButtonPosition(UBoxComponent* ButtonBoxComponent)
{
	return ButtonPositions.Contains(ButtonBoxComponent) ? ButtonPositions[ButtonBoxComponent] : FVector::ZeroVector;
}

void ARubikCube::CreateStickerAndButton(UStaticMeshComponent* PieceMeshComponent, const FVector& Position, const EStickerType StickerType)
{
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
	StickerMeshComponent->SetStaticMesh(StickerMesh);
	StickerMeshComponent->SetMaterial(0, StickerMaterials[StickerType]);
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

	ButtonBoxComponents.Emplace(ButtonBoxComponent);
	ButtonPositions.Emplace(ButtonBoxComponent, Position);
}

void ARubikCube::TurnNext()
{
	if (SignQueue.IsEmpty())
	{
		UE_LOG(LogRubikCube, Log, TEXT("회전 완료 및 패턴 : %s"), *Facelets)
		bIsTurning = false;

		if (HasAuthority())
		{
			if (bIsScrambling)
			{
				UE_LOG(LogRubikCube, Log, TEXT("섞기 완료"))
				bIsScrambling = false;
				FinishScrambleDelegate.Broadcast();
			}

			FString ErrorMessage;
			if (URubikCubeSolver::CheckSolved(Facelets, ErrorMessage))
			{
				UE_LOG(LogRubikCube, Log, TEXT("풀기 완료"))
				FinishSolveDelegate.Broadcast();
				bIsSolved = true;
			}
			else
			{
				if (ErrorMessage.StartsWith(TEXT("ERROR")))
				{
					UE_LOG(LogRubikCube, Error, TEXT("%s"), *ErrorMessage);
				}
				bIsSolved = false;
			}
		}
		
		return;
	}

	FSignInfo NextSign;
	SignQueue.Dequeue(NextSign);

	TurnCore(NextSign);
}

void ARubikCube::TurnCore(const FSignInfo& SignInfo)
{
	GrabPieces(SignInfo);

	const float TargetAngle = SignInfo.TurnCount == 2 ? 180.0f : SignInfo.CCW ? -90.0f : 90.0f;
	
	CurrentTargetQuat = FQuat::Identity;
	switch (SignInfo.CubeAxisType)
	{
	case ECubeAxisType::X:
		CurrentTargetQuat = FRotator(0.0f, 0.0f, TargetAngle).Quaternion();
		break;
        
	case ECubeAxisType::Y:
		CurrentTargetQuat = FRotator(TargetAngle, 0.0f, 0.0f).Quaternion();
		break;
        
	case ECubeAxisType::Z:
		CurrentTargetQuat = FRotator(0.0f, -TargetAngle, 0.0f).Quaternion();
		break;
	}
	
	CurrentSignInfo = SignInfo;
	bIsUpdatingTurnCore = true;
}

void ARubikCube::GrabPieces(const FSignInfo& SignInfo)
{
	for (const auto PiecePosition : PiecePositions)
	{
		switch (SignInfo.CubeAxisType)
		{
		case ECubeAxisType::X:
			if (PiecePosition.Value.X == SignInfo.Layer)
			{
				PiecePosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepWorldTransform);
			}
			break;
		
		case ECubeAxisType::Y:
			if (PiecePosition.Value.Y == SignInfo.Layer)
			{
				PiecePosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepWorldTransform);
			}
			break;
		
		case ECubeAxisType::Z:
			if (PiecePosition.Value.Z == SignInfo.Layer)
			{
				PiecePosition.Key->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepWorldTransform);
			}
			break;
		}
	}
}

void ARubikCube::ReleasePieces(const FSignInfo& SignInfo)
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
	
	FString TempFacelets = TEXT("");
	for (auto PatternOrderPosition : FaceletOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (PatternOrderPosition == StickerPosition.Value)
			{
				if (StickerPosition.Key->GetMaterial(0) == StickerMaterials[EStickerType::Yellow])
				{
					TempFacelets += 'Y';
				}
				else if (StickerPosition.Key->GetMaterial(0) == StickerMaterials[EStickerType::Red])
				{
					TempFacelets += 'R';
				}
				else if (StickerPosition.Key->GetMaterial(0) == StickerMaterials[EStickerType::Blue])
				{
					TempFacelets += 'B';
				}
				else if (StickerPosition.Key->GetMaterial(0) == StickerMaterials[EStickerType::White])
				{
					TempFacelets += 'W';
				}
				else if (StickerPosition.Key->GetMaterial(0) == StickerMaterials[EStickerType::Orange])
				{
					TempFacelets += 'O';
				}
				else if (StickerPosition.Key->GetMaterial(0) == StickerMaterials[EStickerType::Green])
				{
					TempFacelets += 'G';
				}
				
				break;
			}
		}
	}
	Facelets = TempFacelets;

	CoreComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TurnNext();
}

void ARubikCube::ChangeFacelets(const FString& NewFacelets)
{
	int32 Index = 0;
	for (auto PatternOrderPosition : FaceletOrderPositions)
	{
		for (auto StickerPosition : StickerPositions)
		{
			if (PatternOrderPosition == StickerPosition.Value)
			{
				if (NewFacelets[Index] == TEXT('Y'))
				{
					StickerPosition.Key->SetMaterial(0, StickerMaterials[EStickerType::Yellow]);
				}
				else if (NewFacelets[Index] == TEXT('R'))
				{
					StickerPosition.Key->SetMaterial(0, StickerMaterials[EStickerType::Red]);
				}
				else if (NewFacelets[Index] == TEXT('B'))
				{
					StickerPosition.Key->SetMaterial(0, StickerMaterials[EStickerType::Blue]);
				}
				else if (NewFacelets[Index] == TEXT('W'))
				{
					StickerPosition.Key->SetMaterial(0, StickerMaterials[EStickerType::White]);
				}
				else if (NewFacelets[Index] == TEXT('O'))
				{
					StickerPosition.Key->SetMaterial(0, StickerMaterials[EStickerType::Orange]);
				}
				else if (NewFacelets[Index] == TEXT('G'))
				{
					StickerPosition.Key->SetMaterial(0, StickerMaterials[EStickerType::Green]);
				}

				Index++;
				break;
			}
		}
	}
}

FMatrix ARubikCube::GetRotationMatrix(const FSignInfo& SignInfo)
{
	const int32 Sin = SignInfo.TurnCount == 2 ? 0 : SignInfo.CCW ? -1 : 1;
	const int32 Cos = SignInfo.TurnCount == 2 ? -1 : 0;
			
	switch (SignInfo.CubeAxisType)
	{
	case ECubeAxisType::X:
		return FMatrix(
			FVector(1, 0, 0),
			FVector(0, Cos, -Sin),
			FVector(0, Sin, Cos),
			FVector::ZeroVector
		);

	case ECubeAxisType::Y:
		return FMatrix(
			FVector(Cos, 0, Sin),
			FVector(0, 1, 0),
			FVector(-Sin, 0, Cos),
			FVector::ZeroVector
		);

	case ECubeAxisType::Z:
		return FMatrix(
			FVector(Cos, -Sin, 0),
			FVector(Sin, Cos, 0),
			FVector(0, 0, 1),
			FVector::ZeroVector
		);
	}
	
	return FMatrix::Identity;
}

void ARubikCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARubikCube, bIsSolved)
	DOREPLIFETIME_CONDITION(ARubikCube, Facelets, COND_InitialOnly)
}

void ARubikCube::OnRep_Facelets()
{
	ChangeFacelets(Facelets);
}

void ARubikCube::ServerRPC_Spin_Implementation(const FString& Command)
{
	MulticastRPC_Spin(Command);
	MulticastRPC_RenewalPattern(Facelets);
}

void ARubikCube::MulticastRPC_Spin_Implementation(const FString& Command)
{
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
}

void ARubikCube::ServerRPC_Scramble_Implementation()
{
	if (bIsTurning)
	{
		UE_LOG(LogRubikCube, Warning, TEXT("%s"), TEXT("큐브가 회전 중입니다. 회전이 끝날 때까지 기다려주세요."));
		return;
	}
	
	MulticastRPC_Spin(URubikCubeSolver::GenerateScrambleCommand());
	MulticastRPC_RenewalPattern(Facelets);

	bIsScrambling = true;
}

void ARubikCube::ServerRPC_Solve_Implementation()
{
	if (bIsTurning)
	{
		UE_LOG(LogRubikCube, Warning, TEXT("%s"), TEXT("큐브가 회전 중입니다. 회전이 끝날 때까지 기다려주세요."));
		return;
	}

	FString ErrorMessage;
	const FString Command =  URubikCubeSolver::SolveCube(Facelets, ErrorMessage);
	if (ErrorMessage.StartsWith(TEXT("ERROR")))
	{
		UE_LOG(LogRubikCube, Error, TEXT("%s"), *ErrorMessage);
	}
	else
	{
		UE_LOG(LogRubikCube, Log, TEXT("해법 커맨드 : %s"), *Command);
		MulticastRPC_Spin(Command);
		MulticastRPC_RenewalPattern(Facelets);
	}
}

void ARubikCube::MulticastRPC_RenewalPattern_Implementation(const FString& NewPattern)
{
	if (Facelets != NewPattern)
	{
		UE_LOG(LogRubikCube, Warning, TEXT("%s"), TEXT("패턴 보정"));
		
		Facelets = NewPattern;
	
		ChangeFacelets(NewPattern);
	}
}
