// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/RCN_GameModeBase.h"
#include "UI/RCN_MainMenuWidget.h"
#include "RCN_MainMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_MainMenuGameModeBase : public ARCN_GameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<URCN_MainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
