// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_TimerWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"

void URCN_TimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void URCN_TimerWidget::StartTimer()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		FString TimeString = FString::Printf(TEXT("%.2f"), Time);
		
		TxtPlayTime->SetText(FText::FromString(TimeString));
		Time += 1.0f;
		
	}), 1.0f, true);
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

FString URCN_TimerWidget::GetTimerText() const
{
	return FString::Printf(TEXT("%.2f"), Time);
}
