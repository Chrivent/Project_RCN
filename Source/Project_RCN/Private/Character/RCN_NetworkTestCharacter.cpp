// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RCN_NetworkTestCharacter.h"
#include "Project_RCN/Project_RCN.h"

// Sets default values
ARCN_NetworkTestCharacter::ARCN_NetworkTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARCN_NetworkTestCharacter::BeginPlay()
{
	RCN_LOG(LogRCNNetwrok, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	RCN_LOG(LogRCNNetwrok, Log, TEXT("%s"), TEXT("End"));
	
}

