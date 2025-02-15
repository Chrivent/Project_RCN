// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_SinglePlayerMainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_SinglePlayerMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SinglePlayerStartButtonReleasedHandle();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SinglePlayerStartButton;
};
