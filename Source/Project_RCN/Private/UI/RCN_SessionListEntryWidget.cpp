// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_SessionListEntryWidget.h"

#include "Components/TextBlock.h"

void URCN_SessionListEntryWidget::Setup(const int32 InSessionIndex, const FString& InSessionName, const int32 InPing)
{
	SessionIndex = InSessionIndex;
	SessionNameText->SetText(FText::FromString(InSessionName));
	SessionPingText->SetText(FText::Format(NSLOCTEXT("SessionListItem", "PingFormat", "Ping: {0} ms"), InPing));
}
