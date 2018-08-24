// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BucksAlg.generated.h"

USTRUCT(BlueprintType)
struct FPlayerKeycardInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		class APickup* KeyCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		int32 Keycards = 0;

	void AddKeycard() {
		Keycards += 1;
	}
	int32 GetKeycards() {
		return Keycards;
	}


};