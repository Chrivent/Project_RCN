// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RCN_PlayerController.generated.h"

class URCN_TimerWidget;
class URCN_UIDataAsset;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARCN_PlayerController();

	// HUD Section
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_UIDataAsset> UIDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<URCN_TimerWidget> TimerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TObjectPtr<URCN_TimerWidget> TimerWidget;

	void CreateTimerWidget();
	
public:
	// 네트워크 관련
	// 게임과 무관한 액터 초기화
	virtual void PostInitializeComponents() override;
	// 게임과 무관하지만 네트워크 적용을 받는 초기화
	virtual void PostNetInit() override;

	virtual void BeginPlay() override;
	
	// 폰이 빙의될때 호출하는 함수
	virtual void OnPossess(APawn* InPawn) override;
};
