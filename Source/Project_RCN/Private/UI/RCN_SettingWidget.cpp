// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_SettingWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"

void URCN_SettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SettingCategorySoundButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategorySoundButtonReleasedHandle);
	SettingCategoryGraphicButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryGraphicButtonReleasedHandle);
	SettingCategoryKeyBindButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryKeyBindButtonReleasedHandle);
	SettingCategoryEtcButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryEtcButtonReleasedHandle);

	GraphicSettingFullScreenCheckBox->OnCheckStateChanged.AddDynamic(this, &URCN_SettingWidget::GraphicSettingFullScreenCheckBoxCheckStateChangedHandle);
	GraphicSettingComboBox->OnSelectionChanged.AddDynamic(this, &URCN_SettingWidget::ChangeScreenSize);
}

void URCN_SettingWidget::SettingCategorySoundButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(0);
}

void URCN_SettingWidget::SettingCategoryGraphicButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(1);
}

void URCN_SettingWidget::SettingCategoryKeyBindButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(2);
}

void URCN_SettingWidget::SettingCategoryEtcButtonReleasedHandle()
{
	
}

void URCN_SettingWidget::GraphicSettingFullScreenCheckBoxCheckStateChangedHandle(bool bIsChecked)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	EWindowMode::Type NewWindowMode = bIsChecked ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Windowed;

	if (Settings->GetFullscreenMode() != NewWindowMode)
	{
		Settings->SetFullscreenMode(NewWindowMode);
		Settings->ApplySettings(false);

		ChangeScreenSize(GraphicSettingComboBox->GetSelectedOption(), ESelectInfo::Type::OnMouseClick);
		
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Changed Window Mode : %s"), bIsChecked ? TEXT("true") : TEXT("false")));
	}
}

void URCN_SettingWidget::GraphicSettingComboBoxSelectionChangedHandle(FString Resolution, ESelectInfo::Type SelectInfo)
{
	
}

void URCN_SettingWidget::ChangeScreenSize(FString Resolution, ESelectInfo::Type SelectInfo)
{
	FString WidthStr, HeightStr;
	if (!Resolution.Split(TEXT("x"), &WidthStr, &HeightStr))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Wrong Resolution: %s"), *Resolution));
		return;
	}

	if (!WidthStr.IsNumeric() || !HeightStr.IsNumeric())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Resolution must be numeric!"));
		return;
	}
	
	int32 Width = FCString::Atoi(*WidthStr);
	int32 Height = FCString::Atoi(*HeightStr);

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	EWindowMode::Type CurrentWindowMode = Settings->GetFullscreenMode();

	if (CurrentWindowMode == EWindowMode::Type::Fullscreen || CurrentWindowMode == EWindowMode::Type::WindowedFullscreen)
	{
		Settings->RequestResolutionChange(Width, Height, CurrentWindowMode, false);
	}
	else
	{
		Settings->SetScreenResolution(FIntPoint(Width, Height));
	}
	
	Settings->ApplySettings(false);
	Settings->ConfirmVideoMode();
	
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Setting Complete Resolution: %s"), *Resolution));
}
