// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/RCN_MultiPlayerMainMenuWidget.h"
#include "UI/RCN_SettingWidget.h"
#include "UI/RCN_SinglePlayerMainMenuWidget.h"

void URCN_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BackButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::BackButtonReleasedHandle);
	SinglePlayerButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::SinglePlayerButtonReleasedHandle);
	MultiPlayerButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::MultiPlayerButtonReleasedHandle);
	SettingButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::SettingButtonReleasedHandle);
	
	BackButton->SetVisibility(ESlateVisibility::Hidden);
}

void URCN_MainMenuWidget::BackButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(0);

	BackButton->SetVisibility(ESlateVisibility::Hidden);
}

void URCN_MainMenuWidget::SinglePlayerButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidget(SinglePlayerMainMenuWidget);

	BackButton->SetVisibility(ESlateVisibility::Visible);
}

void URCN_MainMenuWidget::MultiPlayerButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidget(MultiPlayerMainMenuWidget);

	BackButton->SetVisibility(ESlateVisibility::Visible);
}

void URCN_MainMenuWidget::SettingButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidget(SettingMainMenuWidget);

	BackButton->SetVisibility(ESlateVisibility::Visible);
}
