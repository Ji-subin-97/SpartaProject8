// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaProject8GameMode.h"
#include "SpartaProject8Character.h"
#include "UObject/ConstructorHelpers.h"

ASpartaProject8GameMode::ASpartaProject8GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
