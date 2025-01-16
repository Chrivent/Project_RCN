// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_TimerWidget.h"

#include "Actor/RCN_RubikCube.h"
#include "Project_RCN/Project_RCN.h"


void URCN_TimerWidget::StartTimer()
{
	UE_LOG(LogTimerStart, Log, TEXT("StartTimer"))
}

void URCN_TimerWidget::StopTimer()
{
	
}

void URCN_TimerWidget::UpdateTimer(float NewTime)
{
	Time = NewTime;

	if (IsValid(TxtPlayTime))
	{
		TxtPlayTime->SetText(FText::FromString(GetTimerText()));
	}
}

FString URCN_TimerWidget::GetTimerText()
{
	return FString::Printf(TEXT("%.2f"), Time);
}

void URCN_TimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}
