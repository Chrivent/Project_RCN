// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_TimerWidget.generated.h"

class UTextBlock;
class ARCN_RubikCube;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_TimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void StartTimer();
	void StopTimer();
	void UpdateTimer(float NewTime);
	FString GetTimerText() const;
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> TxtPlayTime;
	
	UPROPERTY()
	float Time;
};
