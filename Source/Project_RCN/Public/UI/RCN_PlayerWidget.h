// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_PlayerWidget.generated.h"

class URCN_UIDataAsset;
class ARCN_RubikCube;
class ARCN_Player;
class URCN_TimerWidget;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URCN_PlayerWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetPlayer(ARCN_Player* InPlayer) { Player = InPlayer; }

	void BindTimerToRubikCube();

protected:
	virtual void NativeConstruct() override;

	void FinishScrambleHandle();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_UIDataAsset> UIDataAsset;
	
	UPROPERTY()
	TObjectPtr<URCN_TimerWidget> TimerWidget;

	UPROPERTY()
	TObjectPtr<ARCN_Player> Player;
};
