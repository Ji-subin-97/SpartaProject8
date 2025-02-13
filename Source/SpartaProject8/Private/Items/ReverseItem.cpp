// Copyright Epic Games, Inc. All Rights Reserved.


#include "Items/ReverseItem.h"

#include "SpartaCharacter.h"

AReverseItem::AReverseItem()
{
	ItemType = "Reverse";
}

void AReverseItem::ActiveItem(AActor* SourceActor)
{
	Super::ActiveItem(SourceActor);

	if (SourceActor && SourceActor->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(SourceActor))
		{
			PlayerCharacter->Reverse();
		}
		
		DestoryItem();
	}
}


