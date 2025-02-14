// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GameModeBase.h"

#include "Data/RCN_GameModeBaseDataAsset.h"
#include "Project_RCN/Project_RCN.h"
#include "Game/RCN_GameState.h"

ARCN_GameModeBase::ARCN_GameModeBase()
{
	static ConstructorHelpers::FObjectFinder<URCN_GameModeBaseDataAsset> GameModeBaseDataAssetRef(TEXT("/Script/Project_RCN.RCN_GameModeBaseDataAsset'/Game/Data/DA_GameModeBase.DA_GameModeBase'"));
	if (GameModeBaseDataAssetRef.Object)
	{
		GameModeBaseDataAsset = GameModeBaseDataAssetRef.Object;
	}
	else
	{
		RCN_LOG(LogPlayer, Error, TEXT("데이터 에셋 로드 실패"))
		return;
	}
	
	DefaultPawnClass = GameModeBaseDataAsset->DefaultPawnClass;
	PlayerControllerClass = GameModeBaseDataAsset->PlayerControllerClass;

	GameStateClass = ARCN_GameState::StaticClass();
}

void ARCN_GameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("========================================"));
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//ErrorMessage = TEXT("접속 차단");

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* ARCN_GameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
	
	return NewPlayerController;
}

void ARCN_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (IsValid(NetDriver))
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("클라이언트 연결 안됨."));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				RCN_LOG(LogPlayer, Log, TEXT("클라이언트 연결됨 : %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("NetDriver 없음."));
	}

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_GameModeBase::StartPlay()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::StartPlay();

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}
