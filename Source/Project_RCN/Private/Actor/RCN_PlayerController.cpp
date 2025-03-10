// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Data/RCN_UIDataAsset.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_TimerWidget.h"
#include "UI/RCN_MainMenuWidget.h"
#include "UI/RCN_OtherPlayerViewWidget.h"

ARCN_PlayerController::ARCN_PlayerController()
{
	static ConstructorHelpers::FObjectFinder<URCN_UIDataAsset> UIDataAssetRef(TEXT("/Script/Project_RCN.RCN_UIDataAsset'/Game/Data/DA_UI.DA_UI'"));
	if (UIDataAssetRef.Object)
	{
		UIDataAsset = UIDataAssetRef.Object;
	}
	else
	{
		RCN_LOG(LogPlayer, Error, TEXT("UI 데이터 에셋 로드 실패"))
		return;
	}
}

void ARCN_PlayerController::PostInitializeComponents()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::PostNetInit()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
	if (IsValid(NetDriver))
	{
		if (IsValid(NetDriver->ServerConnection))
		{
			RCN_LOG(LogPlayer, Log, TEXT("서버 연결됨 : %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("NetDriver 없음."));
	}

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::BeginPlay()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	SetShowMouseCursor(true);
	
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::OnPossess(APawn* InPawn)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::CreateMainMenu()
{
	MainMenuWidget = CreateWidget<URCN_MainMenuWidget>(this, UIDataAsset->MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void ARCN_PlayerController::CreateTimerWidget()
{
	ClientRPC_CreateTimerWidget();
}

void ARCN_PlayerController::CreateOtherPlayerViewWidget(UTextureRenderTarget2D* RenderTarget)
{
	URCN_OtherPlayerViewWidget* OtherPlayerViewWidget = CreateWidget<URCN_OtherPlayerViewWidget>(this, UIDataAsset->OtherPlayerViewWidgetClass);
	if (!IsValid(OtherPlayerViewWidget))
	{
		RCN_LOG(LogPlayer, Error, TEXT("새로운 UI 위젯 생성 실패"))
		return;
	}

	OtherPlayerViewWidget->AddToViewport();
	OtherPlayerViewWidget->SetOtherPlayerView(RenderTarget);

	const int32 PlayerCount = PlayerViewWidgets.Num();
	const float Y = PlayerCount * (180 + 20) + 20;
	OtherPlayerViewWidget->SetRenderTranslation(FVector2D(0, Y));

	PlayerViewWidgets.Add(OtherPlayerViewWidget);
}

void ARCN_PlayerController::ClientRPC_CreateTimerWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));
	
	TimerWidget = CreateWidget<URCN_TimerWidget>(this, UIDataAsset->TimerWidgetClass);
	TimerWidget->AddToViewport();

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}
