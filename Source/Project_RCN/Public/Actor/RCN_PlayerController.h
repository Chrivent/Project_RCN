// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RCN_PlayerController.generated.h"

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
	
	FORCEINLINE URCN_TimerWidget* GetTimerWidget() { return TimerWidget; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Info")
	int32 PlayerNumber;
	
	FORCEINLINE void SetPlayerNumber(const int32 InPlayerNumber) { PlayerNumber = InPlayerNumber; }
	FORCEINLINE int32 GetPlayerNumber() const { return PlayerNumber; }
	
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
	void CreateSessionListEntryWidget(UListView* SessionListView, const TSharedPtr<FOnlineSessionSearch>& SessionSearch);
	void CreateOtherPlayerViewWidget(UTextureRenderTarget2D* RenderTarget);

	void RequestReturnToMenu();
	
protected:
	void UpdateMoveImage(UImage* Image, FVector2D TargetTranslation);
	void UpdateScaleImage(UImage* Image, FVector2D TargetScale);

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
	TArray<URCN_OtherPlayerViewWidget*> OtherPlayerViewWidgets;
	
	// 네트워크 로직
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateTimerWidget();

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateMultiPlayerGreenRoomWidget();
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_RequestReturnToMenu();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DestroyCube();
};
