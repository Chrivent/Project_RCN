// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_FriendEntryWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;
class FOnlineFriend;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_FriendEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NicknameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> InviteButton;

	void Setup(const TSharedRef<FOnlineFriend>& OnlineFriend);
	
	FUniqueNetIdRepl FriendId;

	UFUNCTION()
	void InviteButtonReleasedHandle();
};
