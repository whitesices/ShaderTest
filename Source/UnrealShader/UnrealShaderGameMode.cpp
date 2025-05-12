// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealShaderGameMode.h"
#include "UnrealShaderCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealShaderGameMode::AUnrealShaderGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
