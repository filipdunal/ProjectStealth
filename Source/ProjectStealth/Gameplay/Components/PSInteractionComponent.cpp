// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSInteractionComponent.h"
#include "PSInteractTriggerComponent.h"


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
		OverlappedTriggers.Sort([this](const TObjectPtr<UPSInteractTriggerComponent>& A, const TObjectPtr<UPSInteractTriggerComponent>& B)
		{
			if (A && IsValid(A) && B && IsValid(B))
			{
				return FVector::Distance(GetComponentLocation(), A->GetOwner()->GetActorLocation()) < FVector::Distance(GetComponentLocation(), A->GetOwner()->GetActorLocation());
			}
			return false;
		});

		CurrentTrigger = OverlappedTriggers[0];
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
