// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.f;
	ExplosionRadius = 300.f;
	ExplosionDamage = 35;
	ItemType = "Mine";
	bIsExploded = false;
	
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollison"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName("OverlapAllDynamic");
	ExplosionCollision->SetupAttachment(SceneComponent);
	
}

void AMineItem::ActiveItem(AActor* Activater)
{
	if (bIsExploded) return;
	
	Super::ActiveItem(Activater);
	
	// 게임 월드 -> 타이머 매니저
	// 타이머 핸들러 : 각자의 시계
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay, false);

	bIsExploded = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* ParticleSystem = nullptr; 
	
	if (ExplosionParticle)
	{
		ParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), false);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	
	TArray<AActor*> OverlappingActors;
	// 오버랩된 액터들을 배열에 넣어줌
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor && OverlappingActor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(OverlappingActor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		}
	}
	
	if (ParticleSystem)
	{
		FTimerHandle DestroyParticleTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimerHandle, 
			[WeakParticle = TWeakObjectPtr<USceneComponent>(ParticleSystem)]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			}, 
			2.0f, false);
	}

	DestoryItem();
}