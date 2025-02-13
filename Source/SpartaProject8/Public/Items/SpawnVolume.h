// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT8_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();
		
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> SpawnRandomItem(int32 Amount);
	
	// 스폰되는 수 반환
	int32 GetSpawnCount() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Spawning")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Spawning")
	UBoxComponent* SpawnArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Spawning")
	int32 SpawnCount;
	// Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Spawning")
	UDataTable* SpawnItemDataTable;
	
	virtual void BeginPlay() override;
	
private:
	// 아이템이 생성될 랜덤 위치 반환
	FVector GetItemSpawnLocation() const;
	// 아이템 생성
	AActor* SpawnItem(TSubclassOf<AActor> Item);
	// 랜덤 아이템 선택
	FItemSpawnRow* GetRandomItemSpawnRow() const;
	
};
