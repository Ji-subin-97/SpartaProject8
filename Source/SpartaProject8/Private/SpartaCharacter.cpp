// Copyright Epic Games, Inc. All Rights Reserved.
#include "SpartaCharacter.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SpartaAbilitySystemComponent.h"
#include "SpartaAttributeSet.h"
#include "SpartaController.h"
#include "SpartaGameState.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1인칭 카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	
	// 1인칭 카메라에 붙는 Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(Camera);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObj(TEXT("/Game/FirstPersonArms/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms"));
	if (SkeletalMeshObj.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SkeletalMeshObj.Object);
	}

	// 스켈레탈 매쉬 그림자 비활성화
	SkeletalMesh->SetCastShadow(false);

	NormalSpeed = 600.f;
	SprintMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintMultiplier;

	// 디버프 여부
	bIsSlow = false;
	bIsReverse = false;
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ASC 및 Attributes 등록
	ASpartaPlayerState* PS = GetSpartaPlayerState();

	if (PS)
	{
		// ASC 사용 등록
		AbilitySystemComponent = Cast<USpartaAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Attributes
		AttributeSet = PS->GetSpartaAttributeSet();
		
	}
}

void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Input Component -> 향상된 입력 시스템으로 수정.
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaController* PlayerController = Cast<ASpartaController>(GetController()))
		{
			// Move
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
			}
			// Look
			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
			}
			// Jump
			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartJump);
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopJump);
			}
			// Sprint
			if (PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartSprint);
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
			}
		}
	}
	
}

void ASpartaCharacter::AddHealth(float Amount)
{
	float CurHealth = GetHealth();
	CurHealth = FMath::Clamp<float>(CurHealth + Amount, 0.f, GetMaxHealth());
	SetHealth(CurHealth);
}

// 이동속도 감소 디버프
void ASpartaCharacter::SlowDown(float Percent)
{
	if (Percent <= 0.f) return;

	bIsSlow = true;
	
	// 기존속도 저장
	float OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float OrIginalNormalSpeed = NormalSpeed;
	
	// 속도 저하
	GetCharacterMovement()->MaxWalkSpeed *= Percent / 100.f;
	NormalSpeed *= Percent / 100.f;
	SprintSpeed = OrIginalNormalSpeed * SprintMultiplier * Percent / 100.f;
	
	// 3초후 원상복구
	if (IsValid(this) && GetCharacterMovement())
	{
		GetWorld()->GetTimerManager().SetTimer(SlowDownTimer, [this, OriginalSpeed, OrIginalNormalSpeed]()
		{
			bIsSlow = false;
			GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
			NormalSpeed = OrIginalNormalSpeed;
			SprintSpeed = NormalSpeed * SprintMultiplier;
		}, 3.f, false);
	}
}

// 이동방향 반대 디버프
void ASpartaCharacter::Reverse()
{
	bIsReverse = true;
	
	// 3초후 원상복구
	if (IsValid(this) && GetCharacterMovement())
	{
		GetWorld()->GetTimerManager().SetTimer(ReverseTimer, [this]()
		{
			bIsReverse = false;
		}, 3.f, false);
	}
}

float ASpartaCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float CurHealth = GetHealth();
	CurHealth -= ActualDamage;

	if (CurHealth <= 0.f)
	{
		OnDeath();
	}
	else
	{
		SetHealth(CurHealth);
	}
	
	return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
	// 게임종료+
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

// ------------------------------ Input Action Start ------------------------------ //

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D MoveInput = value.Get<FVector2D>(); 

	// 만약 혼란 디버프가 존재하면 반대로
	if (bIsReverse)
	{
		MoveInput *= -1.f;
	}
	
	// X축 이동
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	// Y축 이동
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

	// Attributes 테스트
	/*float Health = GetHealth();
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);*/
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (!Controller) return;

	const bool bIsJump = value.Get<bool>();
	if (bIsJump)
	{
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!Controller) return;

	const bool bIsJump = value.Get<bool>();
	if (!bIsJump)
	{
		StopJumping();
	}
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (!Controller || !GetCharacterMovement()) return;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (!Controller || !GetCharacterMovement()) return;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

// ------------------------------  Input Action End  ------------------------------ //

// ------------------------------ Player State Start ------------------------------ //
ASpartaPlayerState* ASpartaCharacter::GetSpartaPlayerState() const
{
	return Cast<ASpartaPlayerState>(GetPlayerState());	
}

float ASpartaCharacter::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

void ASpartaCharacter::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

float ASpartaCharacter::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

// ------------------------------  Player State End  ------------------------------ //
