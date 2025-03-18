// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Actor/RCN_Player.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Data/RCN_UIDataAsset.h"
#include "Game/RCN_GreenRoomModeBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_TimerWidget.h"
#include "UI/RCN_MainMenuWidget.h"
#include "UI/RCN_MultiPlayerGreenRoomWidget.h"
#include "UI/RCN_OtherPlayerViewWidget.h"
#include "UI/RCN_SessionListEntryWidget.h"

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

void ARCN_PlayerController::CreateSessionListEntryWidget(UListView* SessionListView, const TSharedPtr<FOnlineSessionSearch>& SessionSearch)
{
	for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
	{
		URCN_SessionListEntryWidget* SessionItem = CreateWidget<URCN_SessionListEntryWidget>(GetWorld(), UIDataAsset->SessionListEntryWidgetClass);
		SessionItem->Setup(i, SessionSearch->SearchResults[i].Session.OwningUserName, SessionSearch->SearchResults[i].PingInMs);
		SessionListView->AddItem(SessionItem);
	}
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

		ServerRPC_DestroyCube();
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

	CurrentTranslation.X += UIDataAsset->CubeOtherPlayerViewWidgetWidthMoveDistance;
	OtherPlayerViewWidget->GetOtherPlayerView()->SetRenderTranslation(CurrentTranslation);
	OtherPlayerViewWidget->GetOtherPlayerView()->SetRenderScale(FVector2D::ZeroVector);

	for (const auto ExistingOtherPlayerViewWidget : OtherPlayerViewWidgets)
	{
		FVector2D ExistingCurrentTranslation = OtherPlayerViewWidget->GetOtherPlayerView()->GetRenderTransform().Translation;
		ExistingCurrentTranslation.Y += UIDataAsset->CubeOtherPlayerViewWidgetHeightMoveDistance;
		UpdateMoveImage(ExistingOtherPlayerViewWidget->GetOtherPlayerView(), ExistingCurrentTranslation);
	}

	CurrentTranslation.X -= UIDataAsset->CubeOtherPlayerViewWidgetWidthMoveDistance;
	UpdateMoveImage(OtherPlayerViewWidget->GetOtherPlayerView(), CurrentTranslation);
	UpdateScaleImage(OtherPlayerViewWidget->GetOtherPlayerView(), FVector2D(1.0f, 1.0f));
	
	OtherPlayerViewWidgets.Add(OtherPlayerViewWidget);
}

void ARCN_PlayerController::UpdateMoveImage(UImage* Image, const FVector2D TargetTranslation)
{
	const FVector2D CurrentTranslation = Image->GetRenderTransform().Translation;
	const FVector2D NewTranslation = FMath::Lerp(CurrentTranslation, TargetTranslation, UIDataAsset->ImageMoveSpeed);
	Image->SetRenderTranslation(NewTranslation);

	if (NewTranslation.Equals(TargetTranslation))
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
	const FVector2D NewScale = FMath::Lerp(CurrentScale, TargetScale, UIDataAsset->ImageScaleSpeed);
	Image->SetRenderScale(NewScale);

	if (NewScale.Equals(TargetScale))
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

void ARCN_PlayerController::ServerRPC_DestroyCube_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	if (const ARCN_Player* CurrentPlayer = Cast<ARCN_Player>(GetPawn()))
	{
		if (ARCN_GreenRoomModeBase* LobbyModeBase = Cast<ARCN_GreenRoomModeBase>(GetWorld()->GetAuthGameMode()))
		{
			LobbyModeBase->UpdateDestroyCube(CurrentPlayer->GetRubikCube());
		}
	}
	
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}
