// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RCN_OtherPlayerViewWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_OtherPlayerViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOtherPlayerView(UTextureRenderTarget2D* RenderTarget, float OpacitySpeed);
	
protected:
	void UpdateOpacity(UMaterialInstanceDynamic* DynamicMaterial, float TargetOpacity, float OpacitySpeed);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> OtherPlayerView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* RenderTargetMaterial;
};
