// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_SinglePlayerMainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void URCN_SinglePlayerMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SinglePlayerStartButton->OnReleased.AddDynamic(this, &URCN_SinglePlayerMainMenuWidget::SinglePlayerStartButtonReleasedHandle);
}

void URCN_SinglePlayerMainMenuWidget::SinglePlayerStartButtonReleasedHandle()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("SingleLevel"));
}
