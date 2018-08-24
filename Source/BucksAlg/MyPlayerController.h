// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BUCKSALG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = State)
		void BP_ChangeState_Spectator();

	UFUNCTION(BlueprintCallable, Category = State)
		void BP_ChangeState_Player();


};
	
	
