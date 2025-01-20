// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_MultiModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "Project_RCN/Project_RCN.h"

void ARCN_MultiModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass)))
	{
		RubikCube->SetReplicates(true);
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
		{
			if (ARCN_Player* Player = Cast<ARCN_Player>(NewPlayer->GetPawn()))
			{
				Player->SetRubikCube(RubikCube);
				Player->InitCube();

				Player->UpdateCubeRotation(GameModeBaseDataAsset->CubeStartRotation);
			}
			
			for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				if (ARCN_Player* MultiPlayer = Cast<ARCN_Player>(Iterator->Get()->GetPawn()))
				{
					const float Offset = GameModeBaseDataAsset->CubeMultiOffset;
					
					MultiPlayer->UpdateCubeLocation(
						FVector::ForwardVector * GameModeBaseDataAsset->CubeStartDistance +
						FVector(0.0f,
							Iterator.GetIndex() * Offset - (GetWorld()->GetNumPlayerControllers() - 1) * Offset / 2.0f,
							0.0f));
					MultiPlayer->RenewalRubikCubePattern();
				}
			}
		}), 1.0f, false);
	}
}

void ARCN_MultiModeBase::FinishScramble()
{
	Super::FinishScramble();

	RCN_LOG(LogNetwork, Log, TEXT("Scramble"))
}
