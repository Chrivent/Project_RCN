// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/RCN_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "RCN_MultiPlayerGreenRoomWidget.generated.h"

class UTextBlock;

class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_MultiPlayerGreenRoomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetStartOrReadyButtonText(const FString& InString) const { StartOrReadyButtonText->SetText(FText::FromString(InString)); }
	FORCEINLINE void SetStartOrReadyButtonColor(const FLinearColor NewColor) const { StartOrReadyButtonText->SetColorAndOpacity(NewColor); }
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BackButtonReleasedHandle();

	UFUNCTION()
	void StartOrReadyButtonReleasedHandle();
	
	void DestroyedSessionsHandle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartOrReadyButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StartOrReadyButtonText;
};
