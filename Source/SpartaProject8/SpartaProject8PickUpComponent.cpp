// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaProject8PickUpComponent.h"

USpartaProject8PickUpComponent::USpartaProject8PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void USpartaProject8PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &USpartaProject8PickUpComponent::OnSphereBeginOverlap);
}

void USpartaProject8PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ASpartaProject8Character* Character = Cast<ASpartaProject8Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
