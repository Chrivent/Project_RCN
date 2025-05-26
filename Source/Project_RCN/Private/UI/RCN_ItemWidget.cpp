// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_ItemWidget.h"

#include "Components/Button.h"

void URCN_ItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UseButton->OnReleased.AddDynamic(this, &URCN_ItemWidget::UseButtonReleasedHandle);
}

void URCN_ItemWidget::UseButtonReleasedHandle()
{
	
}
