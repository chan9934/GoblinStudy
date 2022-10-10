// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealStudyCGameModeBase.h"
#include "MyCharacter.h"


AUnrealStudyCGameModeBase::AUnrealStudyCGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
}