// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/CoinItem.h"

#include "SpartaGameState.h"

ACoinItem::ACoinItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PointValue = 0;
	ItemType = "DefaultCoinItem";

	RotateSpeed = 30.f;
}

void ACoinItem::ActiveItem(AActor* SourceActor)
{
	Super::ActiveItem(SourceActor);

	if (SourceActor && SourceActor->ActorHasTag("Player"))
	{
		// GameState 점수 업데이트
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = Cast<ASpartaGameState>(World->GetGameState()))
			{
				GameState->OnCoinCollected(PointValue);
			}
		}
	}

	DestoryItem();
}

void ACoinItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotateSpeed > 0)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += RotateSpeed * DeltaTime;
		SetActorRotation(NewRotation);
	}
}




