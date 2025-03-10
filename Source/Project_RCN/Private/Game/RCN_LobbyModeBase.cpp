// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_LobbyModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"


void ARCN_LobbyModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		if (ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass)))
		{
			RubikCube->SetOwner(NewPlayer->GetPawn());
			
			if (ARCN_Player* Player = Cast<ARCN_Player>(NewPlayer->GetPawn()))
			{
				Player->SetRubikCube(RubikCube);
				Player->InitCube();

				if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Player->GetController()))
				{
					int32 NewID = GetAvailableID();
					if (NewID != -1)
					{
						PlayerController->SetPlayerUniqueID(NewID);
						PlayerIDMap.Add(PlayerController, NewID);
					}
					
					/*FString PlayerControllerName = PlayerController->GetFName().ToString();

					FString TempPlayerStringID;
					PlayerControllerName.Split(TEXT("_"), nullptr, &TempPlayerStringID);
					
					int32 PlayerID = FCString::Atoi(*TempPlayerStringID);*/
					
					Player->UpdateCubeLocation(CubeSpawnPosition[PlayerController->GetPlayerUniqueID()]);
					Player->UpdateCubeRotation(GameModeBaseDataAsset->CubeStartRotation);
				}
			}
		}

		for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (ARCN_Player* MultiPlayer = Cast<ARCN_Player>(Iterator->Get()->GetPawn()))
			{
				MultiPlayer->RenewalCube();
			}

			if (Iterator->Get() != NewPlayer)
			{
				const ARCN_PlayerController* OtherPlayerController = Cast<ARCN_PlayerController>(Iterator->Get());
				const ARCN_PlayerController* NewPlayerController = Cast<ARCN_PlayerController>(NewPlayer);
				if (IsValid(OtherPlayerController) && IsValid(NewPlayerController))
				{
					ARCN_Player* OtherPlayer = Cast<ARCN_Player>(OtherPlayerController->GetPawn());
					ARCN_Player* Player = Cast<ARCN_Player>(NewPlayerController->GetPawn());
					if (IsValid(OtherPlayer) && IsValid(Player))
					{
						OtherPlayer->CreateRenderTarget(Player);
						Player->CreateRenderTarget(OtherPlayer);
					}
				}
			}
		}
		
	}), 1.0f, false);
}

void ARCN_LobbyModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (ARCN_Player* Player = Cast<ARCN_Player>(Exiting->GetPawn()))
	{
		if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Player->GetController()))
		{
			int32 ExitingID = PlayerController->GetPlayerUniqueID();
			if (ExitingID != -1)
			{
				ReleaseID(ExitingID);
				PlayerIDMap.Remove(PlayerController);
			}
		}
	}
}

int32 ARCN_LobbyModeBase::GetAvailableID()
{
	if (AvailableIDs.Num() > 0)
	{
		int32 AssignedID = AvailableIDs[0];
		AvailableIDs.RemoveAt(0);
		return AssignedID;
	}

	return -1;
}

void ARCN_LobbyModeBase::ReleaseID(int32 ID)
{
	AvailableIDs.Add(ID);
	AvailableIDs.Sort();
}
