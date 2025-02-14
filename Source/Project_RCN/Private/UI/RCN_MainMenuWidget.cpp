// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/RCN_SinglePlayerMainMenuWidget.h"

void URCN_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SinglePlayerButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::SinglePlayerButtonReleasedHandle);
	BackButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::BackButtonReleasedHandle);
	
	BackButton->SetVisibility(ESlateVisibility::Hidden);
}

void URCN_MainMenuWidget::SinglePlayerButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidget(SinglePlayerMainMenuWidget);

	BackButton->SetVisibility(ESlateVisibility::Visible);
}

void URCN_MainMenuWidget::BackButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(0);

	BackButton->SetVisibility(ESlateVisibility::Hidden);
}
