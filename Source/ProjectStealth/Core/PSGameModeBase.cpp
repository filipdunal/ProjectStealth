// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Core/PSGameModeBase.h"

#include "PSGameStateBase.h"


void APSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PSGameState = GetGameState<APSGameStateBase>();
}


void APSGameModeBase::EndGame(bool bWin)
{
	if(!bIsGameEnded)
	{
		bIsGameEnded = true;
		if (ensure(PSGameState))
		{
			PSGameState->OnGameEnded.Broadcast(bWin);
		}
	}
}