// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_MultiModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ARCN_MultiModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		if (ARCN_PlayerController* NewPlayerController = Cast<ARCN_PlayerController>(NewPlayer))
		{
			NewPlayerController->CreateTimerWidget();
		}
		
		if (ARCN_RubikCube* RubikCube = Cast<ARCN_RubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass)))
		{
			RubikCube->SetOwner(NewPlayer->GetPawn());
			
			if (ARCN_Player* Player = Cast<ARCN_Player>(NewPlayer->GetPawn()))
			{
				Player->SetRubikCube(RubikCube);
				Player->InitCube();

				Player->UpdateCubeLocation(FVector::ForwardVector * GameModeBaseDataAsset->CubeStartDistance);
				Player->UpdateCubeRotation(GameModeBaseDataAsset->CubeStartRotation);
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
							OtherPlayer->CreateOtherPlayerViewWidget(Player);
							Player->CreateOtherPlayerViewWidget(OtherPlayer);
						}
					}
				}
			}

			// Todo: FinishScrambleDelegate를 어떻게 연결할지 생각할 필요가 있음
		}
	}), 1.0f, false);
}

AActor* ARCN_MultiModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);

	for (const auto Actor : Actors)
	{
		if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
		{
			if (PlayerStart->PlayerStartTag == *FString::Printf(TEXT("%d"), GetNumPlayers()))
			{
				return PlayerStart;
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}
