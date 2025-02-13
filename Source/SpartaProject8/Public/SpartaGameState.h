// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

UCLASS()
class SPARTAPROJECT8_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASpartaGameState();
	
	// 레벨 모음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> Levels;
	// 현재 레벨 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	// 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxLevel;
	// 최대 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxWave;
	
	// 플레이어가 코인을 획들할 경우 호출
	UFUNCTION(BlueprintCallable)
	void OnCoinCollected(int32 Score);
	// 게임종료시
	UFUNCTION(BlueprintCallable)
	void OnGameOver();
	
protected:
	
	virtual void BeginPlay() override;

private:
	// 레벨
	int32 Level;
	// 레벨 타이머
	FTimerHandle LevelDurationTimerHandle;
	// HUD 타이머
	FTimerHandle HUDUpdateTimerHandle;
	// 레벨 점수
	int32 LevelScore;
	// 총 생성된 코인 수
	int32 TotalCoinCount;
	// 플레이어가 수집한 코인 수
	int32 CollectedCoinCount;

	// 게임오버 여부
	bool bIsGameOver;

	// 웨이브
	int32 Wave;
	// 웨이브 시작
	void StartWave();
	// 웨이브 종료
	void EndWave();
	// 아이템
	TArray<AActor*> Items;
	
	// 레벨 시작
	void StartLevel();
	// 레벨 종료
	void EndLevel();
	// 레벨 점수 추가
	void AddLevelScore(int32 Score);
	// HUD 업데이트
	void UpdateHUD();
};
