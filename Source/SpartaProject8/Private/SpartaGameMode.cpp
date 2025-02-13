// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaGameMode.h"

#include "SpartaCharacter.h"
#include "SpartaController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaController::StaticClass();
}
