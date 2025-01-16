// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "RCN_TimerWidget.generated.h"

class ARCN_RubikCube;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_TimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetRubikCube(ARCN_RubikCube* InRubikCube) { RubikCube = InRubikCube; }
	
	void StartTimer();
	void StopTimer();
	void UpdateTimer(float NewTime);
	FString GetTimerText();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> TxtPlayTime;

	UPROPERTY()
	TObjectPtr<ARCN_RubikCube> RubikCube;

	UPROPERTY()
	float Time;
};
