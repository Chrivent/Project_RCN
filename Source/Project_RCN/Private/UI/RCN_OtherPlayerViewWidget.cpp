// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_OtherPlayerViewWidget.h"

#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

void URCN_OtherPlayerViewWidget::SetOtherPlayerView(UTextureRenderTarget2D* RenderTarget) const
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(RenderTargetMaterial, GetWorld());
	DynamicMaterial->SetTextureParameterValue(FName("Render Target Texture"), RenderTarget);
	OtherPlayerView->SetBrushFromMaterial(DynamicMaterial);
}
