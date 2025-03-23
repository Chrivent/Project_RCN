// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "RCN_MultiPlayerMainMenuWidget.generated.h"

class UOverlay;
class FOnlineSessionSearch;
class URCN_UIDataAsset;
class UListView;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_MultiPlayerMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetSessionSearchResult(const FOnlineSessionSearchResult& InSessionSearchResult) { SessionSearchResult = InSessionSearchResult; }
	
protected:
	virtual void NativeConstruct() override;

public:
	void VisibleOnNoticeOverlay();
	
protected:
	UFUNCTION()
	void CreateSessionButtonReleasedHandle();
	
	UFUNCTION()
	void FindSessionButtonReleasedHandle();

	UFUNCTION()
	void JoinConfirmButtonReleasedHandle();

	UFUNCTION()
	void JoinCancelButtonReleasedHandle();
	
	void CreatedSessionsHandle() const;
	void FoundSessionsHandle(const TSharedPtr<FOnlineSessionSearch>& SessionSearch) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_UIDataAsset> UIDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> CreateSessionButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> FindSessionButton;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UOverlay> NoticeOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> JoinConfirmButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> JoinCancelButton;

	FOnlineSessionSearchResult SessionSearchResult;
};
