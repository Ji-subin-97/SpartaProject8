// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "SpartaPlayerState.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT8_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float SprintMultiplier;

	UFUNCTION(BlueprintPure)
	ASpartaPlayerState* GetSpartaPlayerState() const;
	UFUNCTION(BlueprintPure)
	float GetHealth() const;
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	void AddHealth(float Amount);
	UFUNCTION(BlueprintCallable)
	void SlowDown(float Percent);
	UFUNCTION(BlueprintCallable)
	void Reverse();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comps|Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comps|Mesh")
	USkeletalMeshComponent* SkeletalMesh;

	// ASC 및 Attributes
	TWeakObjectPtr<USpartaAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<USpartaAttributeSet> AttributeSet;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Input Action
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void StartJump(const FInputActionValue& value);
	void StopJump(const FInputActionValue& value);
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

	void SetHealth(float Health);
	
	void OnDeath();
private:
	float SprintSpeed;
	bool bIsSlow;
	bool bIsReverse;

	FTimerHandle SlowDownTimer;
	FTimerHandle ReverseTimer;
};
