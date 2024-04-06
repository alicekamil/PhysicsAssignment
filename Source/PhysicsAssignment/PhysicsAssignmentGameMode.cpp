// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsAssignmentGameMode.h"
#include "PhysicsAssignmentCharacter.h"
#include "UObject/ConstructorHelpers.h"

APhysicsAssignmentGameMode::APhysicsAssignmentGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
