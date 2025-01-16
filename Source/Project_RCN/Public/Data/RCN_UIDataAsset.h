// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/RCN_PlayerWidget.h"
#include "RCN_UIDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_UIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URCN_PlayerWidget> PlayerWidget;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URCN_TimerWidget> TimerWidget;
};
