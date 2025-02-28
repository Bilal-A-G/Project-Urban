// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_UrbanGameMode.h"
#include "Project_UrbanCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_UrbanGameMode::AProject_UrbanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
