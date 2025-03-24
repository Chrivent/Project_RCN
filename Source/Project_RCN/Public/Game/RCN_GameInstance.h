// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RCN_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetMultiModeBasePlayerNum(const int32 InMultiModeBasePlayerNum) { MultiModeBasePlayerNum = InMultiModeBasePlayerNum; }
	FORCEINLINE int32 GetMultiModeBasePlayerNum() const { return MultiModeBasePlayerNum; }

protected:
	int32 MultiModeBasePlayerNum;
};
