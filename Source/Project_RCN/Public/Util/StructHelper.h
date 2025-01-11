// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructHelper.generated.h"

enum class EAxisType : uint8;

USTRUCT(BlueprintType)
struct FSignInfo
{
	GENERATED_BODY()

	/*FSignInfo() : 
		Sign(TEXT("L")),
		AxisType(EAxisType::AxisX),
		Layer(-1),
		CCW(false),
		TurnCount(1)
	{
	}*/

	UPROPERTY(VisibleAnywhere)
	FString Sign;

	UPROPERTY(VisibleAnywhere)
	EAxisType AxisType;

	UPROPERTY(VisibleAnywhere)
	int32 Layer;

	UPROPERTY(VisibleAnywhere)
	bool CCW;

	UPROPERTY(VisibleAnywhere)
	int32 TurnCount;
};
