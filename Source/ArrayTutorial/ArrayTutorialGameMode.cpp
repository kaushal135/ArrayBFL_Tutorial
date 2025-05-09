// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArrayTutorialGameMode.h"
#include "ArrayTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArrayTutorialGameMode::AArrayTutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
