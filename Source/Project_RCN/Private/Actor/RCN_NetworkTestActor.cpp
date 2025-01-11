// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_NetworkTestActor.h"

// Sets default values
ARCN_NetworkTestActor::ARCN_NetworkTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARCN_NetworkTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARCN_NetworkTestActor::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

