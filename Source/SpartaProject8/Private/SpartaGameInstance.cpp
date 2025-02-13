// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalGameScore = 0;
	CurrentLevel = 0;
}

void USpartaGameInstance::ResetGame()
{
	TotalGameScore = 0;
	CurrentLevel = 0;
}

