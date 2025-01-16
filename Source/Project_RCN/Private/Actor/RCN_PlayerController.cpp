// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_UIDataAsset.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_TimerWidget.h"

ARCN_PlayerController::ARCN_PlayerController()
{
	static ConstructorHelpers::FObjectFinder<URCN_UIDataAsset> UIDataAssetRef(TEXT("/Script/Project_RCN.RCN_UIDataAsset'/Game/Data/DA_UI.DA_UI'"));
	if (UIDataAssetRef.Object)
	{
		UIDataAsset = UIDataAssetRef.Object;
	}
	else
	{
		RCN_LOG(LogNetwork, Error, TEXT("UI 데이터 에셋 로드 실패"))
		return;
	}
	
	/* DataAsset 추가로 인한 변경부분 나중에 확인 받기
	static ConstructorHelpers::FClassFinder<URCN_PlayerWidget> HUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_PlayerWidget.WBP_PlayerWidget_C'"));
	if (IsValid(HUDWidgetRef.Class))
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}*/

	TimerWidgetClass = UIDataAsset->TimerWidgetClass;
}

void ARCN_PlayerController::CreateTimerWidget()
{
	//HUDWidget = CreateWidget<URCN_PlayerWidget>(this, HUDWidgetClass); DataAsset 추가로 인한 변경부분 나중에 확인 받기
	TimerWidget = CreateWidget<URCN_TimerWidget>(this, UIDataAsset->TimerWidgetClass);
	
	if (IsValid(TimerWidget))
	{
		TimerWidget->AddToViewport();

		if (ARCN_Player* Player = Cast<ARCN_Player>(GetPawn()))
		{
			ARCN_RubikCube* RubikCube = Player->GetRubikCube();
			if (IsValid(RubikCube))
			{
				TimerWidget->SetRubikCube(RubikCube);
			}
		}
	}
}

void ARCN_PlayerController::PostInitializeComponents()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::PostNetInit()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
	if (IsValid(NetDriver))
	{
		if (IsValid(NetDriver->ServerConnection))
		{
			RCN_LOG(LogNetwork, Log, TEXT("서버 연결됨 : %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("NetDriver 없음."));
	}

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::BeginPlay()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::OnPossess(APawn* InPawn)
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}
