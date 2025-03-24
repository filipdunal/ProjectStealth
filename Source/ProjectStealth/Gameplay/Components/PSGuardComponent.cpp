// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSGuardComponent.h"

#include "PSTriggerComponent.h"
#include "ProjectStealth/Gameplay/Interfaces/PSTriggerSource.h"

DEFINE_LOG_CATEGORY(LogGuard)

UPSGuardComponent::UPSGuardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GuardState = EPSGuardState::Normal;
	AttentionLevel = 0;
	AttentionIncreaseRate = 1;
	AttentionDecreaseRate = 1;

	SuspicionLevelSettings.Add(EPSSuspicionLevel::High, FPSSuspicionLevelSettings());
	SuspicionLevelSettings.Add(EPSSuspicionLevel::Medium, FPSSuspicionLevelSettings());
	SuspicionLevelSettings.Add(EPSSuspicionLevel::Low, FPSSuspicionLevelSettings());
	SuspicionLevelSettings.Add(EPSSuspicionLevel::None, FPSSuspicionLevelSettings());
}


void UPSGuardComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		GetOwner()->GetComponents<UPSTriggerComponent>(Triggers);
	}

	if(Triggers.IsEmpty())
	{
		UE_LOG(LogGuard, Warning, TEXT("UPSGuardComponent doesn't have any triggers (%s)"), *GetReadableName())
	}

	for(UPSTriggerComponent* Trigger: Triggers)
	{
		Trigger->OnTriggerBegin.AddUniqueDynamic(this, &UPSGuardComponent::OnTriggerBegin);
	}
}


void UPSGuardComponent::OnTriggerBegin(TScriptInterface<IPSTriggerSource> TriggerSource, UPSTriggerComponent* TriggerComponent)
{
	if (UWorld* World = GetWorld())
	{
		if( ! World->GetTimerManager().IsTimerActive(UpdateAttentionHandle))
		{
			World->GetTimerManager().SetTimer(UpdateAttentionHandle, this, &UPSGuardComponent::UpdateAttention, UpdateAttentionRate, true);
		}
	}

	/*
	for (UPSTriggerComponent* Trigger : Triggers)
	{
		if(Trigger != TriggerComponent)
		{
			if(!Trigger->GetTriggerSources().Contains(TriggerSource))
			{
			}
		}
	}
	*/
}


void UPSGuardComponent::UpdateAttention()
{
	TScriptInterface<IPSTriggerSource> MostSuspiciousTriggerSource = nullptr;
	EPSSuspicionLevel SuspicionLevel = EPSSuspicionLevel::None;

	for (const UPSTriggerComponent* Trigger : Triggers)
	{
		for(const TScriptInterface<IPSTriggerSource>& TriggerSource : Trigger->GetTriggerSources())
		{
			if(TriggerSource->GetSuspicionLevel() > SuspicionLevel || MostSuspiciousTriggerSource == nullptr)
			{
				MostSuspiciousTriggerSource = TriggerSource;
				SuspicionLevel = TriggerSource->GetSuspicionLevel();
			}
		}
	}

	if (const FPSSuspicionLevelSettings* Settings = SuspicionLevelSettings.Find(SuspicionLevel))
	{
		const float DesiredAttentionLevel = Settings->AttentionCap;

		if (DesiredAttentionLevel != AttentionLevel)
		{
			if (DesiredAttentionLevel > AttentionLevel)
			{
				AttentionLevel += Settings->AttentionRiseRate * UpdateAttentionRate;
				AttentionLevel = FMath::Min(AttentionLevel, DesiredAttentionLevel);
			}
			else
			{
				AttentionLevel -= AttentionDecreaseRate * UpdateAttentionRate;
				AttentionLevel = FMath::Max(AttentionLevel, DesiredAttentionLevel);
			}

			OnAttentionChanged.Broadcast(AttentionLevel);

			const EPSGuardState OldGuardState = GuardState;
			if(AttentionLevel > 0.66f)
			{
				GuardState = EPSGuardState::Alerted;
			}
			else if(AttentionLevel > 0.33f)
			{
				GuardState = EPSGuardState::Suspicious;
			}
			else
			{
				GuardState = EPSGuardState::Normal;
			}

			if(GuardState != OldGuardState)
			{
				OnGuardStateChanged.Broadcast(GuardState);
			}
		}
	}

	if(MostSuspiciousTriggerSource == nullptr && AttentionLevel == 0.0f)
	{
		if(UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(UpdateAttentionHandle);
		}
	}
}


void UPSGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

