// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SpartaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SPARTAPROJECT8_API USpartaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USpartaAttributeSet();

protected:
	/*virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;*/
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS|Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USpartaAttributeSet, Health)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS|Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USpartaAttributeSet, MaxHealth)
	
};
