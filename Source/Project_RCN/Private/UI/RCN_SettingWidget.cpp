// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_SettingWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void URCN_SettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SettingCategoryAllButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryAllButtonReleasedHandle);
	SettingCategorySoundButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategorySoundButtonReleasedHandle);
	SettingCategoryGraphicButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryGraphicButtonReleasedHandle);
	SettingCategoryKeyBindButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryKeyBindButtonReleasedHandle);
	SettingCategoryEtcButton->OnReleased.AddDynamic(this, &URCN_SettingWidget::SettingCategoryEtcButtonReleasedHandle);
}

void URCN_SettingWidget::SettingCategoryAllButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(0);
}

void URCN_SettingWidget::SettingCategorySoundButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(1);
}

void URCN_SettingWidget::SettingCategoryGraphicButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(2);
}

void URCN_SettingWidget::SettingCategoryKeyBindButtonReleasedHandle()
{
	SettingMenuWidgetSwitcher->SetActiveWidgetIndex(3);
}

void URCN_SettingWidget::SettingCategoryEtcButtonReleasedHandle()
{
	
}
