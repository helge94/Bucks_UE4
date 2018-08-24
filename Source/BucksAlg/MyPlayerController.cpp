// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "BucksAlg.h"
#include "GameFramework/PlayerState.h"

void AMyPlayerController::BP_ChangeState_Spectator()
{
	ChangeState(NAME_Spectating);
	if (Role == ROLE_Authority && PlayerState != NULL)
	{
		PlayerState->bIsSpectator = true;
		PlayerState->bOnlySpectator = true;
	}
}

void AMyPlayerController::BP_ChangeState_Player()
{
	ChangeState(NAME_Playing);
	if (Role == ROLE_Authority && PlayerState != NULL)
	{
		PlayerState->bIsSpectator = false;
		PlayerState->bOnlySpectator = false;
	}
}

