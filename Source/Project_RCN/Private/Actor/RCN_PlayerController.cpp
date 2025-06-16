// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"

#include "OnlineSessionSettings.h"
#include "Actor/RCN_Player.h"
#include "RubikCube.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ScrollBox.h"
#include "Data/RCN_UIDataAsset.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Game/RCN_GreenRoomModeBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_FriendEntryWidget.h"
#include "UI/RCN_ItemWidget.h"
#include "UI/RCN_TimerWidget.h"
#include "UI/RCN_MainMenuWidget.h"
#include "UI/RCN_MultiPlayerGreenRoomWidget.h"
#include "UI/RCN_MultiPlayerMainMenuWidget.h"
#include "UI/RCN_NicknameWidget.h"
#include "UI/RCN_OtherPlayerViewWidget.h"
#include "UI/RCN_SessionListButtonWidget.h"

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

void ARCN_PlayerController::CreateOtherPlayerViewWidget(ARCN_Player* OtherPlayer)
{
	ClientRPC_CreateOtherPlayerViewWidget(OtherPlayer);
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

void ARCN_PlayerController::CreateFriendEntryWidget(UScrollBox* FriendsScrollBox, const TSharedPtr<FOnlineFriend>& OnlineFriend)
{
	if (URCN_FriendEntryWidget* FriendEntryWidget = CreateWidget<URCN_FriendEntryWidget>(this, UIDataAsset->FriendEntryWidgetClass))
	{
		FriendEntryWidget->SetOnlineFriend(OnlineFriend);
		FriendsScrollBox->AddChild(FriendEntryWidget);
	}
}

void ARCN_PlayerController::CreateNicknameWidget(ARCN_Player* OtherPlayer)
{
	ClientRPC_CreateNicknameWidget(OtherPlayer);
}

void ARCN_PlayerController::RemoveInvalidNicknameWidget()
{
	ClientRPC_RemoveInvalidNicknameWidget();
}

void ARCN_PlayerController::CreateItemWidget()
{
	ClientRPC_CreateItemWidget();
}

void ARCN_PlayerController::RemoveItemWidget()
{
	ClientRPC_RemoveItemWidget();
}

void ARCN_PlayerController::GreenRoomStartOrReady()
{
	if (HasAuthority())
	{
		if (ARCN_GreenRoomModeBase* GreenRoomModeBase = Cast<ARCN_GreenRoomModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GreenRoomModeBase->StartGame(this);
		}
	}
	else
	{
		ServerRPC_GreenRoomReady();
	}
}

void ARCN_PlayerController::ChangeGreenRoomReadyButton(const bool bIsReady)
{
	ClientRPC_ChangeGreenRoomReadyButton(bIsReady);
}

void ARCN_PlayerController::ChangeGreenRoomReadyInfo(const int32 PlayerIndex, const bool bIsReady)
{
	ClientRPC_ChangeGreenRoomReadyInfo(PlayerIndex, bIsReady);
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

void ARCN_PlayerController::SessionListButtonReleasedHandle(const FOnlineSessionSearchResult& SessionSearchResult) const
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

void ARCN_PlayerController::ClientRPC_CreateOtherPlayerViewWidget_Implementation(ARCN_Player* OtherPlayer)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	for (const auto OtherPlayerViewWidget : OtherPlayerViewWidgets)
	{
		if (OtherPlayerViewWidget->GetPlayer() == OtherPlayer)
		{
			return;
		}
	}
	
	URCN_OtherPlayerViewWidget* OtherPlayerViewWidget = CreateWidget<URCN_OtherPlayerViewWidget>(this, UIDataAsset->OtherPlayerViewWidgetClass);
	if (!IsValid(OtherPlayerViewWidget))
	{
		RCN_LOG(LogPlayer, Error, TEXT("새로운 UI 위젯 생성 실패"))
		return;
	}

	OtherPlayerViewWidget->AddToViewport();
	OtherPlayerViewWidget->SetPlayer(OtherPlayer);

	USceneCaptureComponent2D* SceneCaptureComponent = NewObject<USceneCaptureComponent2D>(this);
	SceneCaptureComponent->AttachToComponent(OtherPlayer->GetSpringArmComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
	SceneCaptureComponent->RegisterComponent();
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

	UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget->InitAutoFormat(1920, 1080);
	RenderTarget->ClearColor = FLinearColor(0, 0, 0, 1);
	
	AActor* RubikCube = OtherPlayer->GetRubikCube();
	SceneCaptureComponent->ShowOnlyActors.Emplace(RubikCube);
	SceneCaptureComponent->TextureTarget = RenderTarget;
	
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
	OtherPlayerViewWidgets.Emplace(OtherPlayerViewWidget);

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_CreateMultiPlayerGreenRoomWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	MultiPlayerGreenRoomWidget = CreateWidget<URCN_MultiPlayerGreenRoomWidget>(this, UIDataAsset->MultiPlayerGreenRoomWidgetClass);

	if (HasAuthority())
	{
		MultiPlayerGreenRoomWidget->SetStartOrReadyButtonText("Start");
		MultiPlayerGreenRoomWidget->SetColorAndOpacity(FLinearColor::Gray);
	}
	else
	{
		MultiPlayerGreenRoomWidget->SetStartOrReadyButtonText("Ready");
		MultiPlayerGreenRoomWidget->SetColorAndOpacity(FLinearColor::Gray);
	}
	
	MultiPlayerGreenRoomWidget->AddToViewport();
	
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ServerRPC_GreenRoomReady_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));
	
	if (ARCN_GreenRoomModeBase* GreenRoomModeBase = Cast<ARCN_GreenRoomModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GreenRoomModeBase->PlayerReady(this);
	}

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_ChangeGreenRoomReadyButton_Implementation(const bool bIsReady)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	MultiPlayerGreenRoomWidget->SetStartOrReadyButtonColor(bIsReady ? FLinearColor::Blue : FLinearColor::Black);
	
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_ChangeGreenRoomReadyInfo_Implementation(const int32 PlayerIndex, const bool bIsReady)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));
	
	const FString ReadyText = bIsReady ? TEXT("Ready") : TEXT("Not Ready");
	MultiPlayerGreenRoomWidget->ChangePlayerReadyText(PlayerIndex, ReadyText);

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_CreateNicknameWidget_Implementation(ARCN_Player* OtherPlayer)
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	for (const auto NicknameWidget : NicknameWidgets)
	{
		if (NicknameWidget->GetPlayer() == OtherPlayer)
		{
			return;
		}
	}
		
	URCN_NicknameWidget* OtherNicknameWidget = CreateWidget<URCN_NicknameWidget>(this, UIDataAsset->NicknameWidgetClass);
	OtherNicknameWidget->AddToViewport();
	OtherNicknameWidget->SetNicknameText(OtherPlayer->GetPlayerState()->GetPlayerName());
	OtherNicknameWidget->SetPlayer(OtherPlayer);

	FVector2D CurrentTranslation = OtherNicknameWidget->GetRenderTransform().Translation;
	CurrentTranslation.X += 400.0f * NicknameWidgets.Num();
	UpdateMoveWidget(OtherNicknameWidget, CurrentTranslation);

	NicknameWidgets.Emplace(OtherNicknameWidget);

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_RemoveInvalidNicknameWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	for (const auto NicknameWidget : NicknameWidgets)
	{
		if (!IsValid(NicknameWidget->GetPlayer()->Controller))
		{
			NicknameWidget->RemoveFromParent();
			NicknameWidgets.Remove(NicknameWidget);
			break;
		}
	}

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_CreateItemWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	URCN_ItemWidget* NewItemWidget = CreateWidget<URCN_ItemWidget>(this, UIDataAsset->ItemWidgetClass);
	NewItemWidget->AddToViewport();
	ItemWidget = NewItemWidget;

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::ClientRPC_RemoveItemWidget_Implementation()
{
	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("Begin"));

	ItemWidget->RemoveFromParent();

	RCN_LOG(LogPlayer, Log, TEXT("%s"), TEXT("End"));
}
