// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RCN_PlayerController.generated.h"

class URCN_SessionListButtonWidget;
class UWidget;
class FOnlineSessionSearch;
class UListView;
class ARCN_RubikCube;
class URCN_MultiPlayerGreenRoomWidget;
class UImage;
class URCN_MainMenuWidget;
class URCN_OtherPlayerViewWidget;
class ARCN_Player;
class URCN_TimerWidget;
class URCN_UIDataAsset;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARCN_PlayerController();
	
	FORCEINLINE URCN_TimerWidget* GetTimerWidget() const { return TimerWidget; }
	FORCEINLINE TArray<URCN_SessionListButtonWidget*> GetSessionListButtonWidgets() const { return SessionListButtonWidgets; }
	FORCEINLINE URCN_MultiPlayerGreenRoomWidget* GetMultiPlayerGreenRoomWidget() const { return MultiPlayerGreenRoomWidget; }
	
protected:
	// 게임과 무관한 액터 초기화
	virtual void PostInitializeComponents() override;
	
	// 게임과 무관하지만 네트워크 적용을 받는 초기화
	virtual void PostNetInit() override;

	virtual void BeginPlay() override;
	
	// 폰이 빙의될때 호출하는 함수
	virtual void OnPossess(APawn* InPawn) override;

public:
	void CreateMainMenuWidget();
	void CreateMultiPlayerGreenRoomWidget();
	void CreateTimerWidget();
	void CreateSessionListButtonWidget(const TSharedPtr<FOnlineSessionSearch>& SessionSearch);
	void CreateOtherPlayerViewWidget(UTextureRenderTarget2D* RenderTarget);

	void RequestReturnToMenu();
	
protected:
	void UpdateMoveWidget(UWidget* Widget, FVector2D TargetTranslation);
	void UpdateOpacityWidget(UWidget* Widget, float TargetOpacity);
	void SessionListButtonReleasedHandle(const FOnlineSessionSearchResult& SessionSearchResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_UIDataAsset> UIDataAsset;
	
	// MainMenu UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainMenu")
	TObjectPtr<URCN_MainMenuWidget> MainMenuWidget;

	// MultiPlayerGreenRoom UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainMenu")
	TObjectPtr<URCN_MultiPlayerGreenRoomWidget> MultiPlayerGreenRoomWidget;
	
	// Game UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameUI")
	TObjectPtr<URCN_TimerWidget> TimerWidget;

	UPROPERTY(VisibleAnywhere, Category="GameUI")
	TArray<TObjectPtr<URCN_OtherPlayerViewWidget>> OtherPlayerViewWidgets;

	UPROPERTY(VisibleAnywhere, Category="GameUI")
	TArray<TObjectPtr<URCN_SessionListButtonWidget>> SessionListButtonWidgets;
	
	// 네트워크 로직
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateTimerWidget();

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateMultiPlayerGreenRoomWidget();
};
