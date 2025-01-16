// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RCN_PlayerWidget.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_UIDataAsset.h"
#include "Project_RCN/Project_RCN.h"
#include "UI/RCN_TimerWidget.h"

URCN_PlayerWidget::URCN_PlayerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<URCN_UIDataAsset> UIDataAssetRef(TEXT("/Script/Project_RCN.RCN_UIDataAsset'/Game/Data/DA_UI.DA_UI'"));
	if (UIDataAssetRef.Object)
	{
		UIDataAsset = UIDataAssetRef.Object;
	}
	else
	{
		UE_LOG(LogNetwork, Error, TEXT("UI 데이터 에셋 로드 실패"))
		return;
	}
}

void URCN_PlayerWidget::BindTimerToRubikCube()
{
	if (IsValid(Player))
	{
		if (ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(Player->GetRubikCube()))
		{
			UE_LOG(LogTimerStart, Log, TEXT("큐브 바인딩"))
			RubikCube->FinishScrambleDelegate.AddUObject(this, &URCN_PlayerWidget::FinishScrambleHandle);
		}
	}
}

void URCN_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TimerWidget = UIDataAsset->TimerWidget;
	ensure(TimerWidget);
}

void URCN_PlayerWidget::FinishScrambleHandle()
{
	if (IsValid(TimerWidget))
	{
		TimerWidget->StartTimer();
	}
}
