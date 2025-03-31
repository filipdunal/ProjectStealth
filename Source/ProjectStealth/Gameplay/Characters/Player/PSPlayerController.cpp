// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Characters/Player/PSPlayerController.h"

#include "ProjectStealth/Core/PSHUD.h"
#include "ProjectStealth/Gameplay/Characters/PSPlayerCharacter.h"
#include "ProjectStealth/Gameplay/Components/PSInteractionComponent.h"


APSPlayerController::APSPlayerController()
{
	
}


void APSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PSHUD = Cast<APSHUD>(MyHUD);
}


void APSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(APSPlayerCharacter* PlayerCharacter = Cast<APSPlayerCharacter>(InPawn))
	{
		PlayerCharacter->GetInteractionComponent()->OnCurrentTriggerChanged.AddDynamic(this, &APSPlayerController::OnInteractTriggerChanged);
	}
}


void APSPlayerController::OnInteractTriggerChanged(UPSInteractTriggerComponent* OldTrigger, UPSInteractTriggerComponent* NewTrigger)
{
	if(ensure(IsValid(PSHUD)))
	{
		PSHUD->OnInteractTriggerChanged(OldTrigger, NewTrigger);
	}
}