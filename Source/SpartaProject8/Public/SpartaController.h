// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaController.generated.h"

class UInputAction;
class UInputMappingContext;

// Player Controller의 역할
/*
*APlayerController는 플레이어 입력을 처리하고, 카메라 및 UI와 상호작용하는 핵심 클래스입니다.
언리얼 엔진에서 플레이어와 게임 세계(World) 사이의 중재자 역할을 합니다.
 */

UCLASS()
class SPARTAPROJECT8_API ASpartaController : public APlayerController
{
	GENERATED_BODY()

public:
	ASpartaController();
	
	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;
	// Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	// MainMenuWidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main|HUD")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|HUD")
	UUserWidget* MainMenuWidgetInstance;
	
	UFUNCTION(BlueprintPure)
	UUserWidget* GetHUDWidget() const;
	// Game HUD 표시
	UFUNCTION(BlueprintCallable)
	void ShowGameHUD();
	// 메인메뉴 표시
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu(bool bIsRestart);
	// 게임 시작
	UFUNCTION(BlueprintCallable)
	void StartGame();
	// 메인메뉴로 이동
	UFUNCTION(BlueprintCallable)
	void MoveMainMenu();
	
protected:
	virtual void BeginPlay() override;
	
};
