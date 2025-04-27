// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_NicknameWidget.h"

#include "Components/TextBlock.h"

void URCN_NicknameWidget::SetNicknameText(const FString& InNicknameText) const
{
	NicknameText->SetText(FText::FromString(InNicknameText));
}
