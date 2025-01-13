// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RCN_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 게임과 무관한 액터 초기화
	virtual void PostInitializeComponents() override;
	// 게임과 무관하지만 네트워크 적용을 받는 초기화
	virtual void PostNetInit() override;

	virtual void BeginPlay() override;
	
	// 폰이 빙의될때 호출하는 함수
	virtual void OnPossess(APawn* InPawn) override;
};
