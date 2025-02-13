// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpartaGameMode.generated.h"

// GameMode의 역할
/*게임 모드는 게임의 전반적인 규칙과 로직을 담당하는 클래스입니다.
한 게임의 룰을 정의하는 "심판" 역할을 합니다.

✔ 언리얼 엔진의 기본적인 게임 모드 역할

게임 시작 및 종료 로직 관리 (StartPlay(), HandleMatchHasStarted(), HandleMatchHasEnded())
플레이어 스폰 (ChoosePlayerStart(), RestartPlayer())
팀 및 점수 시스템 관리 (ScoreKill(), ScoreObjective())
게임 진행 상태 관리 (GameState와 연동)
 */

UCLASS()
class SPARTAPROJECT8_API ASpartaGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASpartaGameMode();
	
};
