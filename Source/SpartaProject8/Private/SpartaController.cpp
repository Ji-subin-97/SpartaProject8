// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaController.h"

#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaController::ASpartaController() :
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
	
}

void ASpartaController::BeginPlay()
{
	Super::BeginPlay();

	// Input Mapping Context -> SubSystem 등록
	// 1. LocalPlayer 가져오기
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// 2. LocalPlayer Enhanced Input System 가져오기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// 3. EnhancedInputLocalPlayerSubsystem에 IMC 등록
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// 게임 실행 첫 Map 메인메뉴 UI표시
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ASpartaController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaController::ShowGameHUD()
{
	// HUD 켜져있으면 끄기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromViewport();
		HUDWidgetInstance = nullptr;
	}

	// Main메뉴 커져있으면 끄기
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromViewport();
		MainMenuWidgetInstance = nullptr;
	}
	
	// HUD 위젯 생성 표시
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			// 마우스 끄기
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void ASpartaController::ShowMainMenu(bool bIsRestart)
{
	// HUD 켜져있으면 끄기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromViewport();
		HUDWidgetInstance = nullptr;
	}

	// Main메뉴 커져있으면 끄기
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromViewport();
		MainMenuWidgetInstance = nullptr;
	}

	// 메인 메뉴 생성
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			// 마우스 키기
			bShowMouseCursor = true;
			// UI Input모드만 키기
			SetInputMode(FInputModeUIOnly());
		}

		// 재시작 여부에 따른 문구 변경
		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("StartButtonText")))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}

		// 블루프린트로 구현한 문구 추가.
		if (bIsRestart)
		{
			UFunction* PlayerAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayerAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayerAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance()))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalGameScore)));
				}
			}
		}
	}
}

// 게임실행 -> BP
void ASpartaController::StartGame()
{
	// GameInstance 초기화
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance()))
	{
		SpartaGameInstance->ResetGame();
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level1"));
	SetPause(false);
}

// 메인메뉴이동 -> BP
void ASpartaController::MoveMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}



