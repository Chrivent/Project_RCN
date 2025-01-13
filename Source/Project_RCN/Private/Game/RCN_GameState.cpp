// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GameState.h"
#include "Project_RCN/Project_RCN.h"

void ARCN_GameState::HandleBeginPlay()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_GameState::OnRep_ReplicatedHasBegunPlay()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}