// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

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

	if (IsLocalController())
	{
		TimerWidget = CreateWidget<URCN_TimerWidget>(this, UIDataAsset->TimerWidgetClass);
	
		if (IsValid(TimerWidget))
		{
			TimerWidget->AddToViewport();
		}
	}
	
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::OnPossess(APawn* InPawn)
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}
