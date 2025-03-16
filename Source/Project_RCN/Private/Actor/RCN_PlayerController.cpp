// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Data/RCN_UIDataAsset.h"
#include "Game/RCN_LobbyModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_TimerWidget.h"
#include "UI/RCN_MainMenuWidget.h"
#include "UI/RCN_MultiPlayerGreenRoomWidget.h"
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

	PlayerNumber = -1;
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

void ARCN_PlayerController::CreateMainMenuWidget()
{
	MainMenuWidget = CreateWidget<URCN_MainMenuWidget>(this, UIDataAsset->MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void ARCN_PlayerController::CreateMultiPlayerGreenRoomWidget()
{
	ClientRPC_CreateMultiPlayerGreenRoomWidget();
}

void ARCN_PlayerController::CreateTimerWidget()
{
	ClientRPC_CreateTimerWidget();
}

void ARCN_PlayerController::RequestReturnToMenu()
{
	if (HasAuthority())
	{
		for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Iterator->Get()))
			{
				PlayerController->ClientRPC_RequestReturnToMenu();
			}
		}
	}
	else
	{
		ClientRPC_RequestReturnToMenu();
	}
}

void ARCN_PlayerController::ClientRPC_RequestReturnToMenu_Implementation()
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

		SessionInterface->DestroySession(NAME_GameSession);
		
		ClientTravel(TEXT("/Game/Level/MainMenuLevel"), TRAVEL_Absolute);
	}
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
	FVector2D CurrentTranslation = OtherPlayerViewWidget->GetOtherPlayerView()->GetRenderTransform().Translation;
	// Todo: 상수 데이터화 필요
	CurrentTranslation.X += 500.0f;
	OtherPlayerViewWidget->GetOtherPlayerView()->SetRenderTranslation(CurrentTranslation);
	OtherPlayerViewWidget->GetOtherPlayerView()->SetRenderScale(FVector2D::ZeroVector);

	for (const auto ExistingOtherPlayerViewWidget : OtherPlayerViewWidgets)
	{
		FVector2D ExistingCurrentTranslation = OtherPlayerViewWidget->GetOtherPlayerView()->GetRenderTransform().Translation;
		// Todo: 상수 데이터화 필요
		ExistingCurrentTranslation.Y += 200.0f;
		UpdateMoveImage(ExistingOtherPlayerViewWidget->GetOtherPlayerView(), ExistingCurrentTranslation);
	}

	// Todo: 상수 데이터화 필요
	CurrentTranslation.X -= 500.0f;
	UpdateMoveImage(OtherPlayerViewWidget->GetOtherPlayerView(), CurrentTranslation);
	UpdateScaleImage(OtherPlayerViewWidget->GetOtherPlayerView(), FVector2D(1.0f, 1.0f));
	
	OtherPlayerViewWidgets.Add(OtherPlayerViewWidget);
}

void ARCN_PlayerController::UpdateMoveImage(UImage* Image, const FVector2D TargetTranslation)
{
	const FVector2D CurrentTranslation = Image->GetRenderTransform().Translation;
	const FVector2D NewTranslation = FMath::Lerp(CurrentTranslation, TargetTranslation, 0.5f);
	Image->SetRenderTranslation(NewTranslation);

	if (NewTranslation.Equals(TargetTranslation, 0.01f))
	{
		Image->SetRenderTranslation(TargetTranslation);
		return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateMoveImage(Image, TargetTranslation);
	}));
}

void ARCN_PlayerController::UpdateScaleImage(UImage* Image, const FVector2D TargetScale)
{
	const FVector2D CurrentScale = Image->GetRenderTransform().Scale;
	const FVector2D NewScale = FMath::Lerp(CurrentScale, TargetScale, 0.5f);
	Image->SetRenderScale(NewScale);

	if (NewScale.Equals(TargetScale, 0.01f))
	{
		Image->SetRenderScale(TargetScale);
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateScaleImage(Image, TargetScale);
	}));
}

void ARCN_PlayerController::ClientRPC_CreateTimerWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));
	
	TimerWidget = CreateWidget<URCN_TimerWidget>(this, UIDataAsset->TimerWidgetClass);
	TimerWidget->AddToViewport();

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_CreateMultiPlayerGreenRoomWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	MultiPlayerGreenRoomWidget = CreateWidget<URCN_MultiPlayerGreenRoomWidget>(this, UIDataAsset->MultiPlayerGreenRoomWidgetClass);
	MultiPlayerGreenRoomWidget->AddToViewport();
	
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}
