// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "RCN_SessionListButtonWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_SessionListButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetSessionSearchResult(const FOnlineSessionSearchResult& InSessionSearchResult) { SessionSearchResult = InSessionSearchResult; }

protected:
	virtual void NativeConstruct() override;

public:
	void SetSessionSearchResult2(const FOnlineSessionSearchResult& InSessionSearchResult);

protected:
	UFUNCTION()
	void JoinSessionButtonReleasedHandle();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SessionNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SessionPingText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> JoinSessionButton;

	FOnlineSessionSearchResult SessionSearchResult;
};
