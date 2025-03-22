// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_MultiPlayerGreenRoomWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FStartOrReadyDelegate)

class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_MultiPlayerGreenRoomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FStartOrReadyDelegate StartOrReadyDelegate;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BackButtonReleasedHandle();

	UFUNCTION()
	void StartOrReadyButtonReleasedHandle();
	
	void DestroyedSessionsHandle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(EditAnywhere, blueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartOrReadButton;
};
