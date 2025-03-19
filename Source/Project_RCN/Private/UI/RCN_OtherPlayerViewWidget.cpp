// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_OtherPlayerViewWidget.h"

#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

void URCN_OtherPlayerViewWidget::SetOtherPlayerView(UTextureRenderTarget2D* RenderTarget, float OpacitySpeed)
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(RenderTargetMaterial, GetWorld());
	DynamicMaterial->SetTextureParameterValue(FName("Render Target Texture"), RenderTarget);
	DynamicMaterial->SetScalarParameterValue(FName("Opacity"), 0.0f);
	OtherPlayerView->SetBrushFromMaterial(DynamicMaterial);

	UpdateOpacity(DynamicMaterial, 1.0f, OpacitySpeed);
}

void URCN_OtherPlayerViewWidget::UpdateOpacity(UMaterialInstanceDynamic* DynamicMaterial, const float TargetOpacity, const float OpacitySpeed)
{
	float CurrentOpacity;
	DynamicMaterial->GetScalarParameterValue(FHashedMaterialParameterInfo(FName("Opacity")), CurrentOpacity);
	const float NewOpacity = FMath::Lerp(CurrentOpacity, TargetOpacity, OpacitySpeed);
	DynamicMaterial->SetScalarParameterValue(FName("Opacity"), NewOpacity);

	if (FMath::IsNearlyEqual(CurrentOpacity, TargetOpacity))
	{
		DynamicMaterial->SetScalarParameterValue(FName("Opacity"), TargetOpacity);
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateOpacity(DynamicMaterial, TargetOpacity, OpacitySpeed);
	}));
}
