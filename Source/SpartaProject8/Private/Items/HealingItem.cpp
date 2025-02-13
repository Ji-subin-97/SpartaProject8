// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/HealingItem.h"
#include "SpartaCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActiveItem(AActor* Activater)
{
	Super::ActiveItem(Activater);
	
	if (Activater && Activater->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activater))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		
		DestoryItem();
	}
}
