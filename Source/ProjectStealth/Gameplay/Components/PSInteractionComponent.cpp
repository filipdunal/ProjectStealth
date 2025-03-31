// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSInteractionComponent.h"
#include "PSInteractTriggerComponent.h"

#define ECR_InteractionComponent ECC_GameTraceChannel3
#define ECR_TriggerZone ECC_GameTraceChannel2


UPSInteractionComponent::UPSInteractionComponent(const FObjectInitializer& ObjectInitializer)
{
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionObjectType(ECR_InteractionComponent);
	SetCollisionResponseToChannel(ECR_TriggerZone, ECR_Overlap);
}


void UPSInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddUniqueDynamic(this, &UPSInteractionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddUniqueDynamic(this, &UPSInteractionComponent::OnEndOverlap);
}


void UPSInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(UPSInteractTriggerComponent* Trigger = Cast<UPSInteractTriggerComponent>(OtherComp))
	{
		OverlappedTriggers.AddUnique(Trigger);
		UpdateCurrentTrigger();
	}
}


void UPSInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UPSInteractTriggerComponent* Trigger = Cast<UPSInteractTriggerComponent>(OtherComp))
	{
		OverlappedTriggers.Remove(Trigger);
		UpdateCurrentTrigger();
	}
}


void UPSInteractionComponent::UpdateCurrentTrigger()
{
	UPSInteractTriggerComponent* PreviousTrigger = CurrentTrigger;

	// Remove deleted objects
	OverlappedTriggers.RemoveAll([](const TObjectPtr<UPSInteractTriggerComponent>& Object)
	{
		return !IsValid(Object);
	});

	if (OverlappedTriggers.Num() > 0)
	{
		UPSInteractTriggerComponent* ClosestTrigger = OverlappedTriggers[0];
		for(UPSInteractTriggerComponent* Trigger: OverlappedTriggers)
		{
			if(FVector::Distance(GetComponentLocation(), Trigger->GetComponentLocation()) < FVector::Distance(GetComponentLocation(), ClosestTrigger->GetComponentLocation()))
			{
				ClosestTrigger = Trigger;
			}
		}
		CurrentTrigger = ClosestTrigger;
		GetWorld()->GetTimerManager().SetTimer(UpdateCurrentTriggerHandle, this, &UPSInteractionComponent::UpdateCurrentTrigger, 0.5f, false);
	}
	else
	{
		CurrentTrigger = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(UpdateCurrentTriggerHandle);
	}

	if(CurrentTrigger != PreviousTrigger)
	{
		OnCurrentTriggerChanged.Broadcast(IsValid(PreviousTrigger) ? PreviousTrigger : nullptr, CurrentTrigger);
	}
}


void UPSInteractionComponent::StartPrimaryInteract()
{
	if(IsValid(CurrentTrigger))
	{
		CurrentTrigger->CallTriggerBegin(GetOwner());
	}
}


void UPSInteractionComponent::CompletePrimaryInteract()
{
	if(IsValid(CurrentTrigger))
	{
		CurrentTrigger->CallTriggerEnd(GetOwner());
	}
}
