﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RCN_PlayerDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API URCN_PlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> HoldAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> RotateAction;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ScrambleAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> SolveAction;

	UPROPERTY(EditDefaultsOnly)
	float RotateSensitivity;
};