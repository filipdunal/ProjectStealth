// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSInteractTriggerComponent.h"

#define ECR_InteractionComponent ECC_GameTraceChannel3


UPSInteractTriggerComponent::UPSInteractTriggerComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECR_InteractionComponent, ECR_Overlap);
}
