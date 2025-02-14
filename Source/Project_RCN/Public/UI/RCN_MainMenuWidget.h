// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_MainMenuWidget.generated.h"

class UWidgetSwitcher;
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
	void SinglePlayerButtonReleasedHandle();

	UFUNCTION()
	void BackButtonReleasedHandle();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> MainMenuWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> BackButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SinglePlayerButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<URCN_SinglePlayerMainMenuWidget> SinglePlayerMainMenuWidget;
};
