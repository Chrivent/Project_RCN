// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_SingleModeBase.h"
#include "Actor/RCN_Player.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"

DEFINE_LOG_CATEGORY(LogRCNSingleModeBase);

ARCN_SingleModeBase::ARCN_SingleModeBase()
{
	static  ConstructorHelpers::FObjectFinder<URCN_GameModeBaseDataAsset> GameModeBaseDataAssetRef(TEXT("/Script/Project_RCN.RCN_GameModeBaseDataAsset'/Game/Data/DA_GameModeBase.DA_GameModeBase'"));
	if (GameModeBaseDataAssetRef.Object)
	{
		GameModeBaseDataAsset = GameModeBaseDataAssetRef.Object;
	}
	else
	{
		UE_LOG(LogRCNSingleModeBase, Error, TEXT("GameModeBaseLoadFail"));
	}

	DefaultPawnClass = GameModeBaseDataAsset->DefaultPawnClass;
	PlayerControllerClass = GameModeBaseDataAsset->PlayerControllerClass;
}

void ARCN_SingleModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnPlayer();
}

void ARCN_SingleModeBase::SpawnPlayer()
{
	ARCN_Player* Player = Cast<ARCN_Player>(GetWorld()->SpawnActor(GameModeBaseDataAsset->DefaultPawnClass));

	if (IsValid(Player))
	{
		ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass));
		
		Player->SetRubikCube(RubikCube);
	}
}
