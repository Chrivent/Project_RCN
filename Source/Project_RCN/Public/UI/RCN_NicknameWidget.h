// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_NicknameWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_NicknameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNicknameText(const FString& InNicknameText) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NicknameText;
};
