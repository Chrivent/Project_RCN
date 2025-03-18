// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GreenRoomModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "Game/RCN_GameInstance.h"
#include "Project_RCN/Project_RCN.h"
#include "Project_RCN/Public/Utility/SessionManager.h"


void ARCN_GreenRoomModeBase::PostLogin(APlayerController* NewPlayer)
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
						GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("PlayerNumberMap Added: [%s][%d]"), *PlayerNumberMap.FindKey(PlayerNumber)->GetName(), PlayerNumberMap[PlayerController]));
					}
					
					Player->UpdateCubeLocation(GameModeBaseDataAsset->GreenRoomCubeSpawnPosition[PlayerNumberMap[PlayerController]]);
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

void ARCN_GreenRoomModeBase::Logout(AController* Exiting)
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Exiting))
	{
		ReleasePlayerNumber(PlayerController->GetPlayerNumber());
		PlayerNumberMap.Remove(PlayerController);
	}

	// 호스트 마이그레이션 제작중
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
	
	RCN_LOG(LogTemp, Log, TEXT("Logout"))
	
	Super::Logout(Exiting);
}

void ARCN_GreenRoomModeBase::RequestLogout(AController* Exiting)
{
	Logout(Exiting);
}

void ARCN_GreenRoomModeBase::UpdateDestroyCube(ARCN_RubikCube* RubikCube)
{
	const FVector CurrentCubeScale = RubikCube->GetActorScale3D();
	const FVector NewCubeScale  = FMath::Lerp(CurrentCubeScale, FVector::ZeroVector, GameModeBaseDataAsset->CubeDestroySpeed);
	RubikCube->SetActorScale3D(NewCubeScale);

	if (NewCubeScale.Equals(FVector::ZeroVector, 0.01f))
	{
		RubikCube->Destroy();
		return;
	}
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateDestroyCube(RubikCube);
	}));
}

int32 ARCN_GreenRoomModeBase::GetAvailablePlayerNumber()
{
	if (AvailablePlayerNumbers.Num() > 0)
	{
		const int32 AssignedPlayerNumber = AvailablePlayerNumbers[0];
		AvailablePlayerNumbers.RemoveAt(0);
		return AssignedPlayerNumber;
	}

	return -1;
}

void ARCN_GreenRoomModeBase::ReleasePlayerNumber(int32 PlayerNumber)
{
	AvailablePlayerNumbers.Add(PlayerNumber);
	AvailablePlayerNumbers.Sort();
}

void ARCN_GreenRoomModeBase::PromoteClientToHost(APlayerController* NewHostController)
{
	if (!NewHostController)
	{
		return;
	}

	RCN_LOG(LogTemp, Log, TEXT("새 호스트 후보 : %s"), *NewHostController->GetName())
	
	if (USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->MigrateToHost(NewHostController);
	}
}
