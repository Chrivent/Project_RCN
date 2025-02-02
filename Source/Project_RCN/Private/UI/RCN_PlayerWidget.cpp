// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_PlayerWidget.h"

#include "Components/TextBlock.h"

void URCN_PlayerWidget::StartTimer()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		FString TimeString = FString::Printf(TEXT("%.2f"), Time);
		
		TxtPlayTime->SetText(FText::FromString(TimeString));
		Time += 1.0f;
		
	}), 1.0f, true);
}

void URCN_PlayerWidget::StopTimer()
{
	
}

void URCN_PlayerWidget::UpdateTimer(float NewTime)
{
	Time = NewTime;

	if (IsValid(TxtPlayTime))
	{
		TxtPlayTime->SetText(FText::FromString(GetTimerText()));
	}
}

FString URCN_PlayerWidget::GetTimerText() const
{
	return FString::Printf(TEXT("%.2f"), Time);
}

void URCN_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}
