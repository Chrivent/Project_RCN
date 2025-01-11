// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util/EnumHelper.h"
#include "StructHelper.generated.h"

USTRUCT(BlueprintType)
struct FSignInfo
{
	GENERATED_BODY()

	FSignInfo(FString InSign = "L", const EAxisType InAxisType = EAxisType::AxisX, const int32 InLayer = -1, const bool InCCW = false, const int32 InTurnCount = 1)
	{
		Sign = InSign;
		AxisType = InAxisType;
		Layer = InLayer;
		CCW = InCCW;
		TurnCount = InTurnCount;
	}

	UPROPERTY(VisibleAnywhere)
	FString Sign;

	UPROPERTY(VisibleAnywhere)
	EAxisType AxisType;

	UPROPERTY(VisibleAnywhere)
	int32 Layer;

	UPROPERTY(VisibleAnywhere)
	uint8 CCW : 1;

	UPROPERTY(VisibleAnywhere)
	int32 TurnCount;
};
