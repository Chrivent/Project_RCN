// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_SingleModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "UI/RCN_TimerWidget.h"

void ARCN_SingleModeBase::LoginComplete(ARCN_PlayerController* NewPlayerController)
{
	Super::LoginComplete(NewPlayerController);

	if (ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass)))
	{
		RubikCube->SetOwner(NewPlayerController->GetPawn());
		RubikCube->SetActorScale3D(FVector::ZeroVector);
		UpdateAppearCube(RubikCube);
		
		if (ARCN_Player* NewPlayer = Cast<ARCN_Player>(NewPlayerController->GetPawn()))
		{
			NewPlayer->SetRubikCube(RubikCube);

			NewPlayer->SetCubeLocation(FVector::ForwardVector * GameModeBaseDataAsset->CubeStartDistance);
			NewPlayer->SetCubeRotation(GameModeBaseDataAsset->CubeStartRotation);
		}

		RubikCube->FinishScrambleDelegate.AddUObject(this, &ARCN_SingleModeBase::FinishScramble);
	}

	NewPlayerController->CreateTimerWidget();
}

void ARCN_SingleModeBase::FinishScramble() const
{
	PlayerControllers[0]->GetTimerWidget()->StartTimer();
}
