// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_UIDataAsset.generated.h"

class URCN_SessionListButtonWidget;
class URCN_MultiPlayerGreenRoomWidget;
class URCN_SettingWidget;
class URCN_SinglePlayerMainMenuWidget;
class URCN_MultiPlayerMainMenuWidget;
class URCN_MultiPlayerGreenRoomWidget;
class URCN_MainMenuWidget;
class URCN_OtherPlayerViewWidget;
class URCN_TimerWidget;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_UIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	TSubclassOf<URCN_MainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	TSubclassOf<URCN_MultiPlayerMainMenuWidget> MultiPlayerMainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	TSubclassOf<URCN_SinglePlayerMainMenuWidget> SinglePlayerMainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	TSubclassOf<URCN_SettingWidget> SettingWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="MainMenu")
	TSubclassOf<URCN_MultiPlayerGreenRoomWidget> MultiPlayerGreenRoomWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Game UI")
	TSubclassOf<URCN_TimerWidget> TimerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Game UI")
	TSubclassOf<URCN_OtherPlayerViewWidget> OtherPlayerViewWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Game UI")
	TSubclassOf<URCN_SessionListButtonWidget> SessionListButtonWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Game UI")
	float WidgetMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Game UI")
	float WidgetOpacitySpeed;

	UPROPERTY(EditDefaultsOnly)
	float CubeOtherPlayerViewWidgetHeightMoveDistance;
	
	UPROPERTY(EditDefaultsOnly)
	float CubeOtherPlayerViewWidgetWidthMoveDistance;
};
