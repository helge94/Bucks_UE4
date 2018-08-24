// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BucksAlgGameMode.h"
#include "BucksAlgHUD.h"
#include "BucksAlgCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABucksAlgGameMode::ABucksAlgGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	// use our custom HUD class
	HUDClass = ABucksAlgHUD::StaticClass();
}
