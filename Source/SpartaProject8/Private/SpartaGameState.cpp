// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaGameState.h"

#include "SpartaCharacter.h"
#include "SpartaController.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/CoinItem.h"
#include "Items/SpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

ASpartaGameState::ASpartaGameState()
{
	Level = 0;
	LevelScore = 0;
	TotalCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 20.f;
	MaxLevel = 3;

	// 웨이브 시스템 구현
	Wave = 0;
	MaxWave = 3;
	
	bIsGameOver = false;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

// 레벨 시작
void ASpartaGameState::StartLevel()
{
	// Game HUD 키기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaController* SpartaController = Cast<ASpartaController>(PlayerController))
		{
			SpartaController->ShowGameHUD();
		}
	}
	
	// GameInstance에서 현재 레벨 가져오기
	if (UGameInstance* GameInstance = Cast<UGameInstance>(GetGameInstance()))
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			Level = SpartaGameInstance->CurrentLevel;
		}
	}
	
	// 레벨 시작전 점수, 코인갯수 초기화
	LevelScore = 0;
	TotalCoinCount = 0;
	CollectedCoinCount = 0;
	
	// 웨이브 시작
	StartWave();
	
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle,this,&ASpartaGameState::UpdateHUD, 0.1f, true);
}

// 레벨 종료
void ASpartaGameState::EndLevel()
{
	// 게임오버 상태이면 메인 메뉴, 아니면 다음 레벨
	if (!bIsGameOver)
	{
		Level++;
		
		// 획득한 점수 GameInstance에 저장 및 레벨 상승
		if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
			{
				SpartaGameInstance->CurrentLevel = Level;
				SpartaGameInstance->TotalGameScore += LevelScore;
				
				// 현재레벨 == 최대레벨 일경우
				if (Level == MaxLevel)
				{
					OnGameOver();
					return;
				}
			}
		}
		
		if (Levels.IsValidIndex(Level))
		{
			// 레벨오픈
			UGameplayStatics::OpenLevel(GetWorld(), Levels[Level]);
		}
	}
	else
	{
		OnGameOver();
	}
}

// 코인 획득시
void ASpartaGameState::OnCoinCollected(int32 Score)
{
	CollectedCoinCount++;
	AddLevelScore(Score);

	// 코인 갯수가 총 생성된 코인갯수보다 클경우 다음레벨로 진행 -> 다음웨이브로 넘어가도록 수정
	if (TotalCoinCount > 0 && CollectedCoinCount >= TotalCoinCount)
	{
		EndWave();
	}
}

// 게임 종료시
void ASpartaGameState::OnGameOver()
{
	// 게임종료 -> MainMenu
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaController* SpartaController = Cast<ASpartaController>(PlayerController))
		{
			SpartaController->SetPause(true);
			SpartaController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::AddLevelScore(int32 Score)
{
	LevelScore += Score;
}

// 웨이브 시작
void ASpartaGameState::StartWave()
{
	// 생성되어 있는 SpawnVolume 가져오기
	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), SpawnVolumes);
	
	for (AActor* SpawnVolume : SpawnVolumes)
	{
		if (ASpawnVolume* NewSpawnVolume = Cast<ASpawnVolume>(SpawnVolume))
		{
			// 아이템 스폰
			int32 SpawnCount = NewSpawnVolume->GetSpawnCount();

			// Wave별 아이템 스폰갯수 기본갯수에 추가
			SpawnCount += Wave * 5;
			
			if (SpawnCount > 0)
			{
				Items = NewSpawnVolume->SpawnRandomItem(SpawnCount);
				if (Items.Num() > 0)
				{
					for (int32 i = 0; i < Items.Num(); i++)
					{
						if (Items[i]->IsA(ACoinItem::StaticClass()) && Items[i])
						{
							TotalCoinCount++;
						}
					}
				}
			}
		}
	}

	if (GEngine && TotalCoinCount > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Wave%d 시작!"), Wave + 1));
	}
	
	// Timer 초기화
	GetWorldTimerManager().ClearTimer(LevelDurationTimerHandle);
	// LevelDuration 뒤에 EndLevel 호출로 레벨 종료 -> 웨이브 추가로인해 웨이브 종료로 변경
	GetWorldTimerManager().SetTimer(LevelDurationTimerHandle, this, &ASpartaGameState::EndWave, LevelDuration, false);
}

// 웨이브 종료
void ASpartaGameState::EndWave()
{
	Wave++;
	// 만약 현재 웨이브가 최대 웨이브와 같다면 다음레벨
	if (Wave == MaxWave)
	{
		EndLevel();
	}
	// 아닐경우 다음 웨이브 진행
	else
	{
		StartWave();
	}
}

// HUD 업데이트
void ASpartaGameState::UpdateHUD()
{
	// 플레이어 컨트롤러
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaController* SpartaController = Cast<ASpartaController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaController->GetHUDWidget())
			{
				// 시간
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelDurationTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				// 점수
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							int32 GameScore = SpartaGameInstance->TotalGameScore;
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), GameScore + LevelScore)));
						}
					}
				}
				// 레벨
				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelText"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), Level + 1)));
				}
				// 체력바
				if (UProgressBar* ProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HpBar"))))
				{
					if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(SpartaController->GetPawn()))
					{
						if (ASpartaPlayerState* SpartaPlayerState = Cast<ASpartaPlayerState>(SpartaCharacter->GetPlayerState()))
						{
							ProgressBar->SetPercent(SpartaPlayerState->GetHealth()/ SpartaPlayerState->GetMaxHealth());
						}
					}
				}
			}
		}
	}
}





