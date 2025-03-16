// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_LobbyModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "Game/RCN_GameInstance.h"
#include "Project_RCN/Project_RCN.h"


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
					PlayerController->CreateMultiPlayerGreenRoomWidget();
					
					int32 PlayerNumber = GetAvailablePlayerNumber();
					if (PlayerNumber != -1)
					{
						PlayerController->SetPlayerNumber(PlayerNumber);
						PlayerNumberMap.Add(PlayerController, PlayerNumber);
					}
					
					Player->UpdateCubeLocation(CubeSpawnPosition[PlayerController->GetPlayerNumber()]);
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
		}
	}), 1.0f, false);
}

void ARCN_LobbyModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	/*if (const ARCN_Player* Player = Cast<ARCN_Player>(Exiting->GetPawn()))
	{
		if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Player->GetController()))
		{
			const int32 ExitingPlayerNumber = PlayerController->GetPlayerNumber();
			if (ExitingPlayerNumber != -1)
			{
				ReleasePlayerNumber(ExitingPlayerNumber);
				PlayerNumberMap.Remove(PlayerController);
			}
		}
	}*/
	
	/*if (Exiting->IsLocalController() && Exiting->HasAuthority())
	{
		RCN_LOG(LogTemp, Log, TEXT("호스트가 나가므로 호스트 마이그레이션을 실시합니다."))

		TArray<APlayerController*> RemainingControllers;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* Playercontroller = Iterator->Get();

			if (Playercontroller && Playercontroller != Exiting)
			{
				RemainingControllers.Add(Playercontroller);
			}
		}

		if (RemainingControllers.Num() > 0)
		{
			APlayerController* NetHost = RemainingControllers[0];
			PromoteClientToHost(NetHost);
		}
		else
		{
			RCN_LOG(LogTemp, Log, TEXT("남은 플레이어가 없어서 호스트 마이그레이션 불가능"));
		}
	}*/
}

int32 ARCN_LobbyModeBase::GetAvailablePlayerNumber()
{
	if (AvailablePlayerNumbers.Num() > 0)
	{
		const int32 AssignedPlayerNumber = AvailablePlayerNumbers[0];
		AvailablePlayerNumbers.RemoveAt(0);
		return AssignedPlayerNumber;
	}

	return -1;
}

void ARCN_LobbyModeBase::ReleasePlayerNumber(int32 PlayerNumber)
{
	AvailablePlayerNumbers.Add(PlayerNumber);
	AvailablePlayerNumbers.Sort();
}

void ARCN_LobbyModeBase::PromoteClientToHost(APlayerController* NewHostController)
{
	if (!NewHostController)
	{
		return;
	}

	RCN_LOG(LogTemp, Log, TEXT("새 호스트 후보 : %s"), *NewHostController->GetName())
	
	if (URCN_GameInstance* GameInstance = Cast<URCN_GameInstance>(GetGameInstance()))
	{
		GameInstance->MigrateToHost(NewHostController);
	}
	
}

void ARCN_LobbyModeBase::RequstLogout(AController* Exiting)
{
	Logout(Exiting);
}
