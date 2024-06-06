// Copyright Epic Games, Inc. All Rights Reserved.

#include "PI3GameMode.h"
#include "PI3Character.h"
#include "UObject/ConstructorHelpers.h"

API3GameMode::API3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
