// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_PlayerController.h"
#include <Project_RCN/Project_RCN.h>

void ARCN_PlayerController::PostInitializeComponents()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::PostNetInit()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_PlayerController::BeginPlay()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}