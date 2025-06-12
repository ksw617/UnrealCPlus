// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealCPlusGameMode.h"
#include "UnrealCPlusCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealCPlusGameMode::AUnrealCPlusGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
