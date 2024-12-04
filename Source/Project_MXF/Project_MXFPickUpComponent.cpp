// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_MXFPickUpComponent.h"

UProject_MXFPickUpComponent::UProject_MXFPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UProject_MXFPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UProject_MXFPickUpComponent::OnSphereBeginOverlap);
}

void UProject_MXFPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AProject_MXFCharacter* Character = Cast<AProject_MXFCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
