// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_CyberpunkGameMode.h"
#include "Project_CyberpunkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_CyberpunkGameMode::AProject_CyberpunkGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
