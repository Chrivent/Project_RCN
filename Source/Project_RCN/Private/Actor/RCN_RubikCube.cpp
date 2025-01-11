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

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;

	CoreComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CoreComponent"));
	CoreComponent->SetupAttachment(RootComponent);

	for (int i = 0; i < 26; i++)
	{
		UStaticMeshComponent* PieceComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("PieceComponent_%d"), i));
		PieceComponent->SetupAttachment(DefaultComponent);
		PieceMeshComponents.Emplace(PieceComponent);
	}

	if (RubikCubeDataAssetRef.Object)
	{
		const float Distance = RubikCubeDataAsset->PieceDistance;
		const float Size = RubikCubeDataAsset->PieceSize;
		
		int32 Index = 0;
		for (int32 z = -1; z <= 1; z++)
		{
			for (int32 y = -1; y <= 1; y++)
			{
				for (int32 x = -1; x <= 1; x++)
				{
					if (x == 0 && y == 0 && z == 0)
					{
						continue;
					}
				
					PieceMeshComponents[Index]->SetRelativeLocation(FVector(x * Distance, y * Distance, z * Distance));
					PieceMeshComponents[Index]->SetRelativeScale3D(FVector(Size));
					PieceMeshComponents[Index]->SetStaticMesh(RubikCubeDataAsset->PieceMesh);

					Index++;
				}
			}
		}
	
		SignInfos.Emplace("L", EAxisType::AxisX, -Distance, false, 1);
		SignInfos.Emplace("L'", EAxisType::AxisX, -Distance, true, 1);
		SignInfos.Emplace("L2", EAxisType::AxisX, -Distance, false, 2);
	
		SignInfos.Emplace("M", EAxisType::AxisX, 0, false, 1);
		SignInfos.Emplace("M'", EAxisType::AxisX, 0, true, 1);
		SignInfos.Emplace("M2", EAxisType::AxisX, 0, false, 2);
	
		SignInfos.Emplace("R", EAxisType::AxisX, Distance, false, 1);
		SignInfos.Emplace("R'", EAxisType::AxisX, Distance, true, 1);
		SignInfos.Emplace("R2", EAxisType::AxisX, Distance, false, 2);

		SignInfos.Emplace("B", EAxisType::AxisY, -Distance, false, 1);
		SignInfos.Emplace("B'", EAxisType::AxisY, -Distance, true, 1);
		SignInfos.Emplace("B2", EAxisType::AxisY, -Distance, false, 2);
	
		SignInfos.Emplace("S", EAxisType::AxisY, 0, false, 1);
		SignInfos.Emplace("S'", EAxisType::AxisY, 0, true, 1);
		SignInfos.Emplace("S2", EAxisType::AxisY, 0, false, 2);
	
		SignInfos.Emplace("F", EAxisType::AxisY, Distance, false, 1);
		SignInfos.Emplace("F'", EAxisType::AxisY, Distance, true, 1);
		SignInfos.Emplace("F2", EAxisType::AxisY, Distance, false, 2);

		SignInfos.Emplace("D", EAxisType::AxisZ, -Distance, false, 1);
		SignInfos.Emplace("D'", EAxisType::AxisZ, -Distance, true, 1);
		SignInfos.Emplace("D2", EAxisType::AxisZ, -Distance, false, 2);
	
		SignInfos.Emplace("E", EAxisType::AxisZ, 0, false, 1);
		SignInfos.Emplace("E'", EAxisType::AxisZ, 0, true, 1);
		SignInfos.Emplace("E2", EAxisType::AxisZ, 0, false, 2);
	
		SignInfos.Emplace("U", EAxisType::AxisZ, Distance, false, 1);
		SignInfos.Emplace("U'", EAxisType::AxisZ, Distance, true, 1);
		SignInfos.Emplace("U2", EAxisType::AxisZ, Distance, false, 2);
	}
}

// Called when the game starts or when spawned
void ARCN_RubikCube::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle asdf;
	GetWorldTimerManager().SetTimer(asdf, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		Spin("R");
	}), 1.0f, false);
}

// Called every frame
void ARCN_RubikCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARCN_RubikCube::Spin(const FString& Order)
{
	TArray<FChar> OneOrder;

	TArray<FString> ParsedOrders;
	Order.ParseIntoArray(ParsedOrders, TEXT(" "), true);

	for (const FString& ParsedOrder : ParsedOrders)
	{
		for (auto SignInfo : SignInfos)
		{
			if (ParsedOrder == SignInfo.Sign)
			{
				GrabPieces(SignInfo);
				TurnCore(SignInfo);
			}
		}
	}
}

void ARCN_RubikCube::GrabPieces(const FSignInfo& SignInfo)
{
	for (const auto PieceMeshComponent : PieceMeshComponents)
	{
		switch (SignInfo.AxisType)
		{
		case EAxisType::AxisX:
			if (PieceMeshComponent->GetRelativeLocation().X == SignInfo.Layer)
			{
				PieceMeshComponent->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		
		case EAxisType::AxisY:
			if (PieceMeshComponent->GetRelativeLocation().Y == SignInfo.Layer)
			{
				PieceMeshComponent->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		
		case EAxisType::AxisZ:
			if (PieceMeshComponent->GetRelativeLocation().Z == SignInfo.Layer)
			{
				PieceMeshComponent->AttachToComponent(CoreComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
			break;
		}
	}
}

void ARCN_RubikCube::TurnCore(const FSignInfo& SignInfo)
{
	switch (SignInfo.AxisType)
	{
	case EAxisType::AxisX:
		CoreComponent->AddLocalRotation(FRotator(45 * (SignInfo.CCW ? -1 : 1) * SignInfo.TurnCount, 0, 0));
		break;
        
	case EAxisType::AxisY:
		CoreComponent->AddLocalRotation(FRotator(0, 45 * (SignInfo.CCW ? -1 : 1) * SignInfo.TurnCount, 0));
		break;
        
	case EAxisType::AxisZ:
		CoreComponent->AddLocalRotation(FRotator(0, 0, 45 * (SignInfo.CCW ? -1 : 1) * SignInfo.TurnCount));
		break;
	}
}

void ARCN_RubikCube::UpdateTurn()
{
	const FQuat QuatForAdd = FMath::Lerp(CoreComponent->GetComponentQuat(), TargetQuat, 0.5f);
	CoreComponent->AddLocalRotation(QuatForAdd);

	if (CoreComponent->GetComponentQuat().Equals(TargetQuat, 1.0f))
	{
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(this, &ARCN_RubikCube::UpdateTurn);
}

