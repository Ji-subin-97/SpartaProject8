// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// 콜리전 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(SceneComponent);

	// 스태틱매쉬
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(Collision);

	// 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnEndOverlap);
}

void ABaseItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActiveItem(OtherActor);
	}
}

void ABaseItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ABaseItem::ActiveItem(AActor* SourceActor)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickUpParticle)
	{
		// 파티클 효과 생성
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickUpParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
			); 
	}

	if (Particle)
	{
		// 타이머를 이용한 파티클 삭제
		FTimerHandle DestroyParticleTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimerHandle, 
			[WeakParticle = TWeakObjectPtr<USceneComponent>(Particle)]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			}, 
			2.0f, false);
	}

	if (PickUpSound)
	{
		// 사운드 생성
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickUpSound,
			GetActorLocation()
			);
	}
}

void ABaseItem::DestoryItem()
{
	Destroy();
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}







