// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Gameplay/Components/PSGuardComponent.h"

#include "PSTriggerComponent.h"
#include "Blueprint/UserWidget.h"
#include "ProjectStealth/Gameplay/Interfaces/PSTriggerSource.h"
#include "ProjectStealth/Settings/PSGuardSettings.h"
#include "ProjectStealth/Widgets/HUD/PSGuardWidget.h"

DEFINE_LOG_CATEGORY(LogGuard)

UPSGuardComponent::UPSGuardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GuardState = EPSGuardState::Normal;
	AttentionLevel = 0;
	AttentionDecreaseRate = 0.1f;

	SuspicionLevelSettings.Add(EPSSuspicionLevel::High,   FPSSuspicionLevelSettings(0.2f, 1.0f));
	SuspicionLevelSettings.Add(EPSSuspicionLevel::Low,    FPSSuspicionLevelSettings(0.1f, 0.5f));
	SuspicionLevelSettings.Add(EPSSuspicionLevel::None,   FPSSuspicionLevelSettings(0.0f, 0.0f));

	DelayDecreaseAttentionDuration.Add(EPSGuardState::Alerted, 5.0f);
	DelayDecreaseAttentionDuration.Add(EPSGuardState::Suspicious, 2.0f);
	DelayDecreaseAttentionDuration.Add(EPSGuardState::Normal, 0.0f);
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

	// Add guard widget
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(const UPSGuardSettings* GuardSettings = UPSGuardSettings::StaticClass()->GetDefaultObject<UPSGuardSettings>())
		{
			if(GuardSettings->GuardWidgetClass && !GuardWidget)
			{
				GuardWidget = CreateWidget<UPSGuardWidget>(PlayerController, GuardSettings->GuardWidgetClass);
				GuardWidget->GuardComponent = this;
				GuardWidget->AddToViewport();
			}
		}
	}
}

void UPSGuardComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(GuardWidget)
	{
		GuardWidget->RemoveFromParent();
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
			if(IPSTriggerSource::Execute_GetSuspicionLevel(TriggerSource.GetObject()) > SuspicionLevel || MostSuspiciousTriggerSource == nullptr)
			{
				MostSuspiciousTriggerSource = TriggerSource;
				SuspicionLevel = IPSTriggerSource::Execute_GetSuspicionLevel(TriggerSource.GetObject());
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

				bAttentionWasRising = true;
				CancelDelayDecreasingAttention();
			}
			else
			{
				if(bAttentionWasRising)
				{
					DelayDecreasingAttention();
					bAttentionWasRising = false;
				}

				if(!bAttentionDecreaseDelay)
				{
					AttentionLevel -= AttentionDecreaseRate * UpdateAttentionRate;
					AttentionLevel = FMath::Max(AttentionLevel, DesiredAttentionLevel);
				}
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
		if(const UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(UpdateAttentionHandle);
		}
	}
}


void UPSGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UPSGuardComponent::DelayDecreasingAttention()
{
	const float* DelayPtr = DelayDecreaseAttentionDuration.Find(GuardState);
	const float Delay = DelayPtr ? *DelayPtr : 0.0f;

	if(Delay > 0.0f)
	{
		bAttentionDecreaseDelay = true;

		if (const UWorld* World = GetWorld())
		{
			const FTimerDelegate Delegate = FTimerDelegate::CreateLambda([&]()
				{
					bAttentionDecreaseDelay = false;
				});

			World->GetTimerManager().SetTimer(DelayDecreasingAttentionHandle, Delegate, Delay, false);
		}
	}
}

void UPSGuardComponent::CancelDelayDecreasingAttention()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayDecreasingAttentionHandle);
	}
}

