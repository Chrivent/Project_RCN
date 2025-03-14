// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_MainMenuWidget.generated.h"

class URCN_SettingWidget;
class UWidgetSwitcher;
class URCN_MultiPlayerMainMenuWidget;
class URCN_SinglePlayerMainMenuWidget;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
 	virtual void NativeConstruct() override;

	UFUNCTION()
	void BackButtonReleasedHandle();
	
	UFUNCTION()
	void SinglePlayerButtonReleasedHandle();

	UFUNCTION()
	void MultiPlayerButtonReleasedHandle();

	UFUNCTION()
	void SettingButtonReleasedHandle();

	UFUNCTION()
	void GameOutButtonReleasedHandle();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> MainMenuWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> BackButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SinglePlayerButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
    TObjectPtr<UButton> MultiPlayerButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
    TObjectPtr<UButton> SettingButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
    TObjectPtr<UButton> GameOutButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<URCN_SinglePlayerMainMenuWidget> SinglePlayerMainMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<URCN_MultiPlayerMainMenuWidget> MultiPlayerMainMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<URCN_SettingWidget> SettingMainMenuWidget;
};
