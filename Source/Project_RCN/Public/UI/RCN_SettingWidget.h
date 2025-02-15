// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_SettingWidget.generated.h"

class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_SettingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SettingCategoryAllButtonReleasedHandle();

	UFUNCTION()
	void SettingCategorySoundButtonReleasedHandle();

	UFUNCTION()
	void SettingCategoryGraphicButtonReleasedHandle();

	UFUNCTION()
	void SettingCategoryKeyBindButtonReleasedHandle();
	
	UFUNCTION()
	void SettingCategoryEtcButtonReleasedHandle();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> SettingMenuWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryAllButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategorySoundButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryGraphicButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryKeyBindButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryEtcButton;
};
