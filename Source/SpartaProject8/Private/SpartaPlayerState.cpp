// Copyright Epic Games, Inc. All Rights Reserved.


#include "SpartaPlayerState.h"
#include "SpartaAbilitySystemComponent.h"
#include "SpartaAttributeSet.h"

ASpartaPlayerState::ASpartaPlayerState()
{
	// ASC
	AbilitySystemComponent = CreateDefaultSubobject<USpartaAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Attributes
	AttributeSet = CreateDefaultSubobject<USpartaAttributeSet>(TEXT("AttributeSet"));
	
}

UAbilitySystemComponent* ASpartaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USpartaAttributeSet* ASpartaPlayerState::GetSpartaAttributeSet() const
{
	return AttributeSet;
}

float ASpartaPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float ASpartaPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}
