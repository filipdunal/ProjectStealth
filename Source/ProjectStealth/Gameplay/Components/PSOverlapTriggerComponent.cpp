// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSOverlapTriggerComponent.h"
#include "ProjectStealth/Gameplay/Interfaces/PSTriggerSource.h"

UPSOverlapTriggerComponent::UPSOverlapTriggerComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
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
		OnTriggerBegin.Broadcast(OtherActor, this);
	}
}


void UPSOverlapTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CanTrigger(OtherActor))
	{
		OnTriggerEnd.Broadcast(OtherActor, this);
	}
}