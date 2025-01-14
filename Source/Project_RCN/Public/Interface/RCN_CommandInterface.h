// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RCN_CommandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URCN_CommandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_RCN_API IRCN_CommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FString GetScrambleCommand() = 0;
	virtual FString GetSolveCommand() = 0;
	virtual void Spin(const FString& Command) = 0;
};
