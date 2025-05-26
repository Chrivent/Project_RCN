// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Test/RCN_TestMultiModeBase.h"

#include "Project_RCN/Project_RCN.h"

ARCN_TestMultiModeBase::ARCN_TestMultiModeBase()
{
	SpinnablePlayerNumber = 1;
}

void ARCN_TestMultiModeBase::SpinCube(ARCN_PlayerController* PlayerController, const FString& Command)
{
	RCN_LOG(LogTemp, Log, TEXT("%d"), PlayerNumberMap[PlayerController])

	if (PlayerNumberMap[PlayerController] != SpinnablePlayerNumber)
	{
		return;
	}
	
	Super::SpinCube(PlayerController, Command);

	TArray<int32> PlayerNumbers;
	PlayerNumberMap.GenerateValueArray(PlayerNumbers);

	const int32 CurrentIndex = PlayerNumbers.IndexOfByKey(SpinnablePlayerNumber);
	const int32 NextIndex = (CurrentIndex + 1) % PlayerNumbers.Num();

	SpinnablePlayerNumber = PlayerNumbers[NextIndex];
}
