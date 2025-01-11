// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructHelper.generated.h"

enum class ETurnDirection : uint8;
enum class EAxisType : uint8;

USTRUCT(BlueprintType)
struct FSignInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString Sign;

	UPROPERTY(VisibleAnywhere)
	EAxisType AxisType;

	UPROPERTY(VisibleAnywhere)
	int32 Layer;

	UPROPERTY(VisibleAnywhere)
	ETurnDirection TurnDirection;

	UPROPERTY(VisibleAnywhere)
	int32 TurnCount;
};
