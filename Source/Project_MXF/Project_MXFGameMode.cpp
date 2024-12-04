// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_MXFGameMode.h"
#include "Project_MXFCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_MXFGameMode::AProject_MXFGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
