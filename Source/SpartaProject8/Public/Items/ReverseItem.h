// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "ReverseItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT8_API AReverseItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AReverseItem();

protected:
	virtual void ActiveItem(AActor* SourceActor) override;
	
};
