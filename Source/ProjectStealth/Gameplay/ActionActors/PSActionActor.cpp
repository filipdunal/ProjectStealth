// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/ActionActors/PSActionActor.h"


APSActionActor::APSActionActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = true;
}


void APSActionActor::BeginPlay()
{
	Super::BeginPlay();
}


bool APSActionActor::CanPerformAction_Implementation(const TScriptInterface<IPSTriggerSource>& TriggerSource) const
{
	return IsActive();
}


void APSActionActor::PerformAction_Implementation(const TScriptInterface<IPSTriggerSource>& TriggerSource)
{
}


bool APSActionActor::TryPerformAction(TScriptInterface<IPSTriggerSource> TriggerSource)
{
	if (CanPerformAction(TriggerSource))
	{
		PerformAction(TriggerSource);
		return true;
	}

	return false;
}


void APSActionActor::SetActive(bool bNewActive)
{
	if(bNewActive != bIsActive)
	{
		bIsActive = bNewActive;
		OnActivityChanged.Broadcast(bIsActive);
	}
}


void APSActionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}