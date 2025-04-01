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
	
protected:
	virtual void NativeConstruct() override;
	
public:
	FORCEINLINE void SetStartOrReadyButtonText(const FString& InString) const { StartOrReadyButtonText->SetText(FText::FromString(InString)); }
	FORCEINLINE void SetStartOrReadyButtonColor(const FLinearColor NewColor) const { StartOrReadyButtonText->SetColorAndOpacity(NewColor); }

	void ChangePlayerReadyText(const int32 PlayerIndex, const FString& ReadyText) const;
	
protected:
	UFUNCTION()
	void BackButtonReleasedHandle();

	UFUNCTION()
	void StartOrReadyButtonReleasedHandle();
	
	void DestroyedSessionsHandle() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartOrReadyButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StartOrReadyButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Player1ReadyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Player2ReadyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Player3ReadyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Player4ReadyText;
};
