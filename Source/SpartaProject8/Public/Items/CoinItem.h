// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class SPARTAPROJECT8_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Rotate")
	float RotateSpeed;
	
	virtual void ActiveItem(AActor* SourceActor) override;

	virtual void Tick(float DeltaTime) override;
	
};
