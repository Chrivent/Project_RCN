// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_MultiModeBase.h"

#include "Actor/RCN_Player.h"
#include "Actor/RCN_PlayerController.h"
#include "RubikCube.h"
#include "Data/RCN_GameModeBaseDataAsset.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

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

void ARCN_MultiModeBase::LoginComplete(ARCN_PlayerController* NewPlayerController)
{
	Super::LoginComplete(NewPlayerController);

	if (ARubikCube* RubikCube = Cast<ARubikCube>(GetWorld()->SpawnActor(GameModeBaseDataAsset->RubikCubeClass)))
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

		// Todo: FinishScrambleDelegate를 어떻게 연결할지 생각할 필요가 있음
	}
	
	for (const auto PlayerController : PlayerControllers)
	{
		if (ARCN_Player* Player = Cast<ARCN_Player>(PlayerController->GetPawn()))
		{
			Player->RenewalCube();
		}
	}
	
	NewPlayerController->CreateTimerWidget();
	
	for (const auto PlayerController1 : PlayerControllers)
	{
		for (const auto PlayerController2 : PlayerControllers)
		{
			if (PlayerController1 != PlayerController2)
			{
				if (ARCN_Player* OtherPlayer = Cast<ARCN_Player>(PlayerController2->GetPawn()))
				{
					PlayerController1->CreateOtherPlayerViewWidget(OtherPlayer);
				}
			}
		}
	}
}
