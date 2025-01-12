// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RCN_GameModeBase.h"

#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"

ARCN_GameModeBase::ARCN_GameModeBase()
{
	DefaultPawnClass = ARCN_RubikCube::StaticClass();
	PlayerControllerClass = ARCN_PlayerController::StaticClass();
}
