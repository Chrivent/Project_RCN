// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GreenRoomModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "Project_RCN/Project_RCN.h"
#include "Project_RCN/Public/Utility/SessionManager.h"

void ARCN_GreenRoomModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (int32 i = 0; i < 4; i++)
	{
		AvailablePlayerNumbers.Emplace(i);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		bool bAllCubeScrambled = true;
		for (const auto PlayerCube : PlayerCubeMap)
		{
			if (PlayerCube.Value->IsSolved())
			{
				bAllCubeScrambled = false;
			}
		}
		for (const auto PlayerCube : PlayerCubeMap)
		{
			if (bAllCubeScrambled)
			{
				PlayerCube.Value->Solve();
			}
			else
			{
				PlayerCube.Value->Scramble();
			}
		}
	}), 5.0f, true);
}

void ARCN_GreenRoomModeBase::Logout(AController* Exiting)
{
	if (ARCN_PlayerController* PlayerController = Cast<ARCN_PlayerController>(Exiting))
	{
		UpdateDestroyCube(PlayerCubeMap[PlayerController]);
		PlayerCubeMap.Remove(PlayerController);
		PlayerReadyMap.Remove(PlayerController);
	}
	
	// 호스트 마이그레이션 제작중
	/*if (Exiting->IsLocalController() && Exiting->HasAuthority())
	{
		RCN_LOG(LogTemp, Log, TEXT("호스트가 나가므로 호스트 마이그레이션을 실시합니다."))

		TArray<APlayerController*> RemainingControllers;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();

			if (PlayerController && PlayerController != Exiting)
			{
				RemainingControllers.Add(PlayerController);
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

void ARCN_GreenRoomModeBase::StartGame(ARCN_PlayerController* PressedPlayerController)
{
	PlayerReadyMap[PressedPlayerController] = true;
	
	if (PlayerReadyMap.Num() > 1 && PlayerAllReadCheck())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("ServerTravel : MultiLevel")));
		
		GetWorld()->ServerTravel(TEXT("/Game/Level/MultiLevel?listen"));
	}
}

void ARCN_GreenRoomModeBase::PlayerReady(ARCN_PlayerController* PressedPlayerController)
{
	if (PlayerReadyMap[PressedPlayerController] == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Not Read")));
		PlayerReadyMap[PressedPlayerController] = false;
		
		PressedPlayerController->SetGreenRoomReady(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Ready")));
		PlayerReadyMap[PressedPlayerController] = true;
		
		PressedPlayerController->SetGreenRoomReady(true);
	}
}

void ARCN_GreenRoomModeBase::LoginComplete(ARCN_PlayerController* NewPlayerController)
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

			PlayerCubeMap.Emplace(NewPlayerController, RubikCube);
			PlayerReadyMap.Emplace(NewPlayerController, false);
					
			NewPlayer->SetCubeLocation(GameModeBaseDataAsset->GreenRoomCubeSpawnPosition[PlayerNumberMap[NewPlayerController]]);
			NewPlayer->SetCubeRotation(GameModeBaseDataAsset->CubeStartRotation);
		}
	}

	for (const auto PlayerController : PlayerControllers)
	{
		if (ARCN_Player* Player = Cast<ARCN_Player>(PlayerController->GetPawn()))
		{
			Player->RenewalCube();
		}
	}

	NewPlayerController->CreateMultiPlayerGreenRoomWidget();
}

void ARCN_GreenRoomModeBase::PromoteClientToHost(APlayerController* NewHostController)
{
	if (!NewHostController)
	{
		return;
	}

	RCN_LOG(LogTemp, Log, TEXT("새 호스트 후보 : %s"), *NewHostController->GetName())
	
	if (const USessionManager* SessionManager = GetGameInstance()->GetSubsystem<USessionManager>())
	{
		SessionManager->MigrateToHost(NewHostController);
	}
}

bool ARCN_GreenRoomModeBase::PlayerAllReadCheck()
{
	for (const auto Players : PlayerReadyMap)
	{
		if (Players.Value == false)
        {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Player Not All Ready")));
        	return false;
        }
	}

	return true;
}
