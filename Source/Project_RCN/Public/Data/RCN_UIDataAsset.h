// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_UIDataAsset.generated.h"

class URCN_TimerWidget;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_UIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URCN_TimerWidget> TimerWidgetClass;;
};
