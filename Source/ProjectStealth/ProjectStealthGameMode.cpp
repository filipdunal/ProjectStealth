// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectStealthGameMode.h"
#include "ProjectStealthCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectStealthGameMode::AProjectStealthGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
