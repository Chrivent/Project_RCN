// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_SingleModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "UI/RCN_TimerWidget.h"

void ARCN_SingleModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ARCN_PlayerController* NewPlayerController = Cast<ARCN_PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		NewPlayerController->CreateTimerWidget();
	}

	if (ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass)))
	{
		RubikCube->SetOwner(NewPlayer->GetPawn());
		
		if (ARCN_Player* Player = Cast<ARCN_Player>(NewPlayer->GetPawn()))
		{
			Player->SetRubikCube(RubikCube);

			Player->UpdateCubeLocation(FVector::ForwardVector * GameModeBaseDataAsset->CubeStartDistance);
			Player->UpdateCubeRotation(GameModeBaseDataAsset->CubeStartRotation);
		}

		RubikCube->FinishScrambleDelegate.AddUObject(this, &ARCN_SingleModeBase::FinishScramble);
	}
}

void ARCN_SingleModeBase::FinishScramble() const
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->GetTimerWidget()->StartTimer();
	}
}
