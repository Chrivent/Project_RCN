// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GameModeBase.h"

#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "Project_RCN/Project_RCN.h"
#include "Game/RCN_GameState.h"

DEFINE_LOG_CATEGORY(LogGameModeBase);

ARCN_GameModeBase::ARCN_GameModeBase()
{
	static ConstructorHelpers::FObjectFinder<URCN_GameModeBaseDataAsset> GameModeBaseDataAssetRef(TEXT("/Script/Project_RCN.RCN_GameModeBaseDataAsset'/Game/Data/DA_GameModeBase.DA_GameModeBase'"));
	if (GameModeBaseDataAssetRef.Object)
	{
		GameModeBaseDataAsset = GameModeBaseDataAssetRef.Object;
	}
	else
	{
		RCN_LOG(LogGameModeBase, Error, TEXT("데이터 에셋 로드 실패"))
		return;
	}
	
	DefaultPawnClass = GameModeBaseDataAsset->DefaultPawnClass;
	PlayerControllerClass = GameModeBaseDataAsset->PlayerControllerClass;

	GameStateClass = ARCN_GameState::StaticClass();
}

void ARCN_GameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("========================================"));
	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//ErrorMessage = TEXT("접속 차단");

	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* ARCN_GameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("End"));
	
	return NewPlayerController;
}

void ARCN_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (IsValid(NetDriver))
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("클라이언트 연결 안됨."));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				RCN_LOG(LogGameModeBase, Log, TEXT("클라이언트 연결됨 : %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("NetDriver 없음."));
	}

	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_GameModeBase::StartPlay()
{
	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::StartPlay();

	RCN_LOG(LogGameModeBase, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_GameModeBase::Logout(AController* Exiting)
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Exiting))
	{
		ReleasePlayerNumber(PlayerNumberMap[PlayerController]);
		PlayerControllers.Remove(PlayerController);
		PlayerNumberMap.Remove(PlayerController);
	}
	
	Super::Logout(Exiting);
}

void ARCN_GameModeBase::LoginComplete(ARCN_PlayerController* NewPlayerController)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%s is Login Complete"), *NewPlayerController->GetName()));

	PlayerControllers.Emplace(NewPlayerController);
	PlayerNumberMap.Emplace(NewPlayerController, GetAvailablePlayerNumber());
}

int32 ARCN_GameModeBase::GetAvailablePlayerNumber()
{
	if (AvailablePlayerNumbers.Num() > 0)
	{
		const int32 AssignedPlayerNumber = AvailablePlayerNumbers[0];
		AvailablePlayerNumbers.RemoveAt(0);
		return AssignedPlayerNumber;
	}

	return -1;
}

void ARCN_GameModeBase::ReleasePlayerNumber(int32 PlayerNumber)
{
	AvailablePlayerNumbers.Emplace(PlayerNumber);
	AvailablePlayerNumbers.Sort();
}

void ARCN_GameModeBase::UpdateAppearCube(ARCN_RubikCube* RubikCube)
{
	const FVector CurrentScale = RubikCube->GetActorScale3D();
	const FVector NewScale = FMath::Lerp(CurrentScale, FVector::OneVector, GameModeBaseDataAsset->CubeAppearSpeed);
	RubikCube->SetActorScale3D(NewScale);

	if (NewScale.Equals(FVector::OneVector))
	{
		RubikCube->SetActorScale3D(FVector::OneVector);
		return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateAppearCube(RubikCube);
	}));
}

void ARCN_GameModeBase::UpdateDestroyCube(ARCN_RubikCube* RubikCube)
{
	const FVector CurrentCubeScale = RubikCube->GetActorScale3D();
	const FVector NewCubeScale  = FMath::Lerp(CurrentCubeScale, FVector::ZeroVector, GameModeBaseDataAsset->CubeDestroySpeed);
	RubikCube->SetActorScale3D(NewCubeScale);

	if (NewCubeScale.Equals(FVector::ZeroVector))
	{
		RubikCube->Destroy();
		return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateDestroyCube(RubikCube);
	}));
}
