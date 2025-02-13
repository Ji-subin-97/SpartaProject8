// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/SlowingItem.h"
#include "SpartaCharacter.h"

ASlowingItem::ASlowingItem()
{
	SlowPercentage = 50.f;
	ItemType = "Slowing";
}

void ASlowingItem::ActiveItem(AActor* SourceActor)
{
	Super::ActiveItem(SourceActor);

	if (SourceActor && SourceActor->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(SourceActor))
		{
			PlayerCharacter->SlowDown(SlowPercentage);
		}
		
		DestoryItem();
	}
}

