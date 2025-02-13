// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/SpawnVolume.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(SceneComponent);
	
	// 아이템 스폰 최대갯수
	SpawnCount = 0;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	//SpawnRandomItem(SpawnCount);
}

// 아이템 위치 랜덤생성
FVector ASpawnVolume::GetItemSpawnLocation() const
{
	// Box Component의 위치 및 반지름
	FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnArea->GetComponentLocation();

	FVector RandomLocation = FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
		);
	
	// 생성된 랜덤위치 아래에 플랫폼이 존재하는지 확인 -> Line Trace이용
	FVector StartLocation = FVector(BoxOrigin.X + RandomLocation.X, BoxOrigin.Y + RandomLocation.Y, BoxOrigin.Z+ RandomLocation.Z);
	FVector EndLocation = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z - (BoxExtent.Z * 1.5f));

	FHitResult Hit;

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
		);

	if(bIsHit)
	{
		return Hit.ImpactPoint + FVector(0.f, 0.f, 40.f);
	}
	else
	{
		// Line Trace가 성공할때까지 재귀
		return GetItemSpawnLocation();
	}
}

// 스폰 수 반환
int32 ASpawnVolume::GetSpawnCount() const
{
	return SpawnCount;	
}

// 아이템 생성
AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> Item)
{
	if (!Item) return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		Item,
		GetItemSpawnLocation(),
		FRotator::ZeroRotator
		);
	
	return SpawnedActor;
}

// 아이템 랜덤 생성
TArray<AActor*> ASpawnVolume::SpawnRandomItem(int32 Amount)
{
	TArray<AActor*> SpawnedActors;
	
	for (int32 i = 0; i < Amount; i++)
	{
		if (FItemSpawnRow* SelectedRow = GetRandomItemSpawnRow())
		{
			if (UClass* Item = SelectedRow->ItemClass.Get())
			{
				SpawnedActors.Add(SpawnItem(Item));
			}
		}
	}
	
	return SpawnedActors;
}

// 데이터테이블에서 랜덤 가중치 알고리즘 선택을 이용하여 아이템 가져오기
FItemSpawnRow* ASpawnVolume::GetRandomItemSpawnRow() const
{
	if (!SpawnItemDataTable) return nullptr;
	// 데이터테이블 모든 Row 가져오기
	TArray<FItemSpawnRow*> SpawnRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	SpawnItemDataTable->GetAllRows(ContextString, SpawnRows);

	if (SpawnRows.IsEmpty()) return nullptr;

	// 가중치 랜덤선택 알고리즘
	float TotalChance = 0.f;
	for (const FItemSpawnRow* SpawnRow : SpawnRows)
	{
		if (SpawnRow)
		{
			TotalChance += SpawnRow->SpawnChance;
		}
	}

	const float RandValue = FMath::RandRange(0.f,TotalChance);
	float AccumulateChance = 0.f;

	for (FItemSpawnRow* SpawnRow : SpawnRows)
	{
		AccumulateChance += SpawnRow->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return SpawnRow;
		}
	}

	return nullptr;
}




