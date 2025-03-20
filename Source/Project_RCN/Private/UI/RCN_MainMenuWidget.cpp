// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_MainMenuWidget.h"

#include "Actor/RCN_PlayerController.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/RCN_MultiPlayerMainMenuWidget.h"
#include "UI/RCN_SettingWidget.h"
#include "UI/RCN_SinglePlayerMainMenuWidget.h"
#include "Kismet\KismetSystemLibrary.h"
#include "UI/RCN_SessionListButtonWidget.h"

void URCN_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BackButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::BackButtonReleasedHandle);
	SinglePlayerButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::SinglePlayerButtonReleasedHandle);
	MultiPlayerButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::MultiPlayerButtonReleasedHandle);
	SettingButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::SettingButtonReleasedHandle);
	GameOutButton->OnReleased.AddDynamic(this, &URCN_MainMenuWidget::GameOutButtonReleasedHandle);
	
	BackButton->SetVisibility(ESlateVisibility::Hidden);

	
	// Todo : 추후 수정 필요
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this] {
		UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		
		Settings->ApplySettings(false);
	}), 0.1f, false);
}

void URCN_MainMenuWidget::BackButtonReleasedHandle()
{
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(0);

	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetOwningPlayer()))
	{
		for (auto SessionListButtonWidget : PlayerController->GetSessionListButtonWidgets())
		{
			SessionListButtonWidget->RemoveFromParent();
		}
	}
	
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

void URCN_MainMenuWidget::GameOutButtonReleasedHandle()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
