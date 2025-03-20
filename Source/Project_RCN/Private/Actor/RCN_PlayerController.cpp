// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

#include "OnlineSessionSettings.h"
#include "Actor/RCN_Player.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Data/RCN_UIDataAsset.h"
#include "Game/RCN_GreenRoomModeBase.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_TimerWidget.h"
#include "UI/RCN_MainMenuWidget.h"
#include "UI/RCN_MultiPlayerGreenRoomWidget.h"
#include "UI/RCN_MultiPlayerMainMenuWidget.h"
#include "UI/RCN_OtherPlayerViewWidget.h"
#include "UI/RCN_SessionListButtonWidget.h"
#include "Utility/SessionManager.h"

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

void ARCN_PlayerController::CreateSessionListButtonWidget(const TSharedPtr<FOnlineSessionSearch>& SessionSearch)
{
	for (const auto SessionListButtonWidget : SessionListButtonWidgets)
	{
		SessionListButtonWidget->RemoveFromParent();
	}
	
	for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
	{
		URCN_SessionListButtonWidget* SessionListButtonWidget = CreateWidget<URCN_SessionListButtonWidget>(GetWorld(), UIDataAsset->SessionListButtonWidgetClass);
		if (!IsValid(SessionListButtonWidget))
		{
			RCN_LOG(LogPlayer, Error, TEXT("새로운 UI 위젯 생성 실패"))
			return;
		}
		
		SessionListButtonWidget->AddToViewport(-1);
		SessionListButtonWidget->SetSessionSearchResult(SessionSearch->SearchResults[i]);
		SessionListButtonWidget->SessionListButtonReleasedDelegate.AddUObject(this, &ARCN_PlayerController::SessionListButtonReleasedHandle);
		MainMenuWidget->SetSessionListButtonWidgets(SessionListButtonWidgets);
		
		FVector2D CurrentTranslation = SessionListButtonWidget->GetRenderTransform().Translation;
		// Todo: 상수화 필요
		CurrentTranslation.X += 500.0f;
		CurrentTranslation.Y += 100.0f * i;
		SessionListButtonWidget->SetRenderTranslation(CurrentTranslation);
		SessionListButtonWidget->SetRenderOpacity(0.0f);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
		{
			FVector2D NewTranslation = SessionListButtonWidget->GetRenderTransform().Translation;
			// Todo: 상수화 필요
			NewTranslation.X -= 500.0f;
			UpdateMoveWidget(SessionListButtonWidget, NewTranslation);
			UpdateOpacityWidget(SessionListButtonWidget, 1.0f);
			// Todo: 상수화 필요
		}), i * 0.1f + 0.01f, false);

		SessionListButtonWidgets.Emplace(SessionListButtonWidget);
	}
}

void ARCN_PlayerController::RequestReturnToMenu()
{
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		ServerRPC_DestroyCube();
		
		SessionManager->DestroySession(this);
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
	OtherPlayerViewWidget->SetOtherPlayerView(RenderTarget, UIDataAsset->WidgetOpacitySpeed);
	
	FVector2D CurrentTranslation = OtherPlayerViewWidget->GetRenderTransform().Translation;
	CurrentTranslation.X += UIDataAsset->CubeOtherPlayerViewWidgetWidthMoveDistance;
	OtherPlayerViewWidget->SetRenderTranslation(CurrentTranslation);

	for (const auto ExistingOtherPlayerViewWidget : OtherPlayerViewWidgets)
	{
		FVector2D ExistingCurrentTranslation = OtherPlayerViewWidget->GetRenderTransform().Translation;
		ExistingCurrentTranslation.Y += UIDataAsset->CubeOtherPlayerViewWidgetHeightMoveDistance;
		UpdateMoveWidget(ExistingOtherPlayerViewWidget, ExistingCurrentTranslation);
	}

	CurrentTranslation.X -= UIDataAsset->CubeOtherPlayerViewWidgetWidthMoveDistance;
	UpdateMoveWidget(OtherPlayerViewWidget, CurrentTranslation);
	OtherPlayerViewWidgets.Add(OtherPlayerViewWidget);
}

void ARCN_PlayerController::UpdateMoveWidget(UWidget* Widget, const FVector2D TargetTranslation)
{
	const FVector2D CurrentTranslation = Widget->GetRenderTransform().Translation;
	const FVector2D NewTranslation = FMath::Lerp(CurrentTranslation, TargetTranslation, UIDataAsset->WidgetMoveSpeed);
	Widget->SetRenderTranslation(NewTranslation);

	if (NewTranslation.Equals(TargetTranslation))
	{
		Widget->SetRenderTranslation(TargetTranslation);
		return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateMoveWidget(Widget, TargetTranslation);
	}));
}

void ARCN_PlayerController::UpdateOpacityWidget(UWidget* Widget, const float TargetOpacity)
{
	const float CurrentOpacity = Widget->GetRenderOpacity();
	const float NewOpacity = FMath::Lerp(CurrentOpacity, TargetOpacity, 0.1f);
	Widget->SetRenderOpacity(NewOpacity);

	if (FMath::IsNearlyEqual(CurrentOpacity, TargetOpacity))
	{
		Widget->SetRenderOpacity(TargetOpacity);
		return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateOpacityWidget(Widget, TargetOpacity);
	}));
}

void ARCN_PlayerController::SessionListButtonReleasedHandle(const FOnlineSessionSearchResult& SessionSearchResult)
{
	MainMenuWidget->GetMultiPlayerMainMenuWidget()->SetSessionSearchResult(SessionSearchResult);
	MainMenuWidget->GetMultiPlayerMainMenuWidget()->VisibleOnNoticeOverlay();
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
		if (ARCN_GreenRoomModeBase* GreenRoomModeBase = Cast<ARCN_GreenRoomModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GreenRoomModeBase->UpdateDestroyCube(CurrentPlayer->GetRubikCube());
		}
	}
	
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}
