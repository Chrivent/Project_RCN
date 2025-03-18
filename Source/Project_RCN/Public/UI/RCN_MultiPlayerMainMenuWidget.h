// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_MultiPlayerMainMenuWidget.generated.h"

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
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateSessionButtonReleasedHandle();
	
	UFUNCTION()
	void FindSessionButtonReleasedHandle();

	void OnCreatedSessionsHandle();
	void OnFoundSessionsHandle(const TSharedPtr<FOnlineSessionSearch>& SessionSearch);
	void SessionSelectedHandle(UObject* SelectedItem) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_UIDataAsset> UIDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> CreateSessionButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> FindSessionButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UListView> SessionListView;

	TObjectPtr<FOnlineSessionSearch> LastSessionSearch;
};
