// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSOverlapTriggerComponent.h"
#include "ProjectStealth/Gameplay/Interfaces/PSTriggerSource.h"

#define ECC_TriggerSource ECC_GameTraceChannel1
#define ECC_TriggerZone ECC_GameTraceChannel2


UPSOverlapTriggerComponent::UPSOverlapTriggerComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_TriggerSource, ECR_Overlap);
}


void UPSOverlapTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddUniqueDynamic(this, &UPSOverlapTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddUniqueDynamic(this, &UPSOverlapTriggerComponent::OnEndOverlap);
}


void UPSOverlapTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanTrigger(OtherActor))
	{
		CallTriggerBegin(OtherActor);
	}
}


void UPSOverlapTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CanTrigger(OtherActor))
	{
		CallTriggerEnd(OtherActor);
	}
}