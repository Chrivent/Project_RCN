// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RCN_PlayerController.generated.h"

class URCN_ItemWidget;
class URCN_NicknameWidget;
class FOnlineFriend;
class UScrollBox;
class URCN_GreenRoomReadyInfoWidget;
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
	void CreateOtherPlayerViewWidget(ARCN_Player* OtherPlayer);
	void CreateSessionListButtonWidget(const TSharedPtr<FOnlineSessionSearch>& SessionSearch);
	void CreateFriendEntryWidget(UScrollBox* FriendsScrollBox, const TSharedPtr<FOnlineFriend>& OnlineFriend);
	void CreateNicknameWidget(ARCN_Player* OtherPlayer);
	void RemoveInvalidNicknameWidget();
	void CreateItemWidget();
	void RemoveItemWidget();
	
	void GreenRoomStartOrReady();
	void ChangeGreenRoomReadyButton(const bool bIsReady);
	void ChangeGreenRoomReadyInfo(const int32 PlayerIndex, const bool bIsReady);
	
protected:
	void UpdateMoveWidget(UWidget* Widget, FVector2D TargetTranslation);
	void UpdateOpacityWidget(UWidget* Widget, float TargetOpacity);
	void SessionListButtonReleasedHandle(const FOnlineSessionSearchResult& SessionSearchResult) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_UIDataAsset> UIDataAsset;
	
	// MainMenu UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainMenu")
	TObjectPtr<URCN_MainMenuWidget> MainMenuWidget;

	// MultiPlayerGreenRoom UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GreenRoom")
	TObjectPtr<URCN_MultiPlayerGreenRoomWidget> MultiPlayerGreenRoomWidget;
	
	// Game UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameUI")
	TObjectPtr<URCN_TimerWidget> TimerWidget;

	UPROPERTY(VisibleAnywhere, Category="GameUI")
	TArray<TObjectPtr<URCN_OtherPlayerViewWidget>> OtherPlayerViewWidgets;

	UPROPERTY(VisibleAnywhere, Category="GameUI")
	TArray<TObjectPtr<URCN_SessionListButtonWidget>> SessionListButtonWidgets;

	UPROPERTY(VisibleAnywhere, Category="GameUI")
	TArray<TObjectPtr<URCN_NicknameWidget>> NicknameWidgets;

	UPROPERTY(VisibleAnywhere, Category="GameUI")
	TObjectPtr<URCN_ItemWidget> ItemWidget;
	
	// 네트워크 로직
	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateTimerWidget();

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateOtherPlayerViewWidget(ARCN_Player* OtherPlayer);

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateMultiPlayerGreenRoomWidget();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_GreenRoomReady();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeGreenRoomReadyButton(const bool bIsReady);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ChangeGreenRoomReadyInfo(const int32 PlayerIndex, const bool bIsReady);

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateNicknameWidget(ARCN_Player* OtherPlayer);

	UFUNCTION(Client, Reliable)
	void ClientRPC_RemoveInvalidNicknameWidget();

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateItemWidget();

	UFUNCTION(Client, Reliable)
	void ClientRPC_RemoveItemWidget();
};
