// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BucksAlgGameMode.generated.h"

UCLASS(minimalapi)
class ABucksAlgGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABucksAlgGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGameStateBase* ABase;
};



