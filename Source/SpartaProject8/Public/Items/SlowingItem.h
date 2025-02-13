// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class SPARTAPROJECT8_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ASlowingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SlowPercentage;

protected:
	virtual void ActiveItem(AActor* SourceActor) override;
	
};
