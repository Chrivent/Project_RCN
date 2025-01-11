// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RCN_NetworkTestCharacter.generated.h"

UCLASS()
class PROJECT_RCN_API ARCN_NetworkTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARCN_NetworkTestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
