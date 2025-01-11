// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumHelper.generated.h"

UENUM(BlueprintType)
enum class EAxisType : uint8
{
	AxisX,
	AxisY,
	AxisZ
};
