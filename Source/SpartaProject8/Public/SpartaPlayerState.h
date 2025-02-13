// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "SpartaPlayerState.generated.h"

class USpartaAbilitySystemComponent;
class USpartaAttributeSet;

UCLASS()
class SPARTAPROJECT8_API ASpartaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASpartaPlayerState();

	// ASC 생성
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Attributes 생성
	USpartaAttributeSet* GetSpartaAttributeSet() const;
	
protected:
	UPROPERTY()
	USpartaAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	USpartaAttributeSet* AttributeSet;

public:
	UFUNCTION(BlueprintPure, Category = "GAS|PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "GAS|PlayerState|Attributes")
	float GetMaxHealth() const;
	
};
