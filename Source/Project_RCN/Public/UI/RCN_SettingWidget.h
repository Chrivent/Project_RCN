// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_SettingWidget.generated.h"

class UCheckBox;
class UComboBoxString;
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
	void SettingCategorySoundButtonReleasedHandle();

	UFUNCTION()
	void SettingCategoryGraphicButtonReleasedHandle();

	UFUNCTION()
	void SettingCategoryKeyBindButtonReleasedHandle();
	
	UFUNCTION()
	void SettingCategoryEtcButtonReleasedHandle();

	UFUNCTION()
	void GraphicSettingFullScreenCheckBoxCheckStateChangedHandle(bool bIsChecked);
	
	UFUNCTION()
	void GraphicSettingComboBoxSelectionChangedHandle(FString Resolution, ESelectInfo::Type SelectInfo);

	UFUNCTION()
	void ChangeScreenSize(FString Resolution, ESelectInfo::Type SelectInfo);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> SettingMenuWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategorySoundButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryGraphicButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryKeyBindButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingCategoryEtcButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> GraphicSettingFullScreenCheckBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UComboBoxString> GraphicSettingComboBox;
};
