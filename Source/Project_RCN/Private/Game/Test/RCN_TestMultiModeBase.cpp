// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Test/RCN_TestMultiModeBase.h"

#include "Actor/RCN_PlayerController.h"
#include "Project_RCN/Project_RCN.h"

ARCN_TestMultiModeBase::ARCN_TestMultiModeBase()
{
	SpinnablePlayerNumber = 0;
}

void ARCN_TestMultiModeBase::Logout(AController* Exiting)
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Exiting))
	{
		SpinCountMap.Remove(PlayerController);
		ItemWidgetExistMap.Remove(PlayerController);
	}
	
	Super::Logout(Exiting);
}

void ARCN_TestMultiModeBase::LoginComplete(ARCN_PlayerController* NewPlayerController)
{
	Super::LoginComplete(NewPlayerController);

	SpinCountMap.Emplace(NewPlayerController, 0);
	ItemWidgetExistMap.Emplace(NewPlayerController, false);
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

	SpinCountMap[PlayerController]++;

	if (SpinCountMap[PlayerController] > 3)
	{
		SpinCountMap[PlayerController] = 0;

		PlayerController->CreateItemWidget();
		ItemWidgetExistMap[PlayerController] = true;
	}
	else
	{
		if (ItemWidgetExistMap[PlayerController])
		{
			ItemWidgetExistMap[PlayerController] = false;
			PlayerController->RemoveItemWidget();
		}
	}
}
