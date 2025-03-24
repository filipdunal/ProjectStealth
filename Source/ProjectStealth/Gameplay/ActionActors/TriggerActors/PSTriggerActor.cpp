// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#include "ProjectStealth/Gameplay/ActionActors/TriggerActors/PSTriggerActor.h"

#include "Components/BoxComponent.h"
#include "ProjectStealth/Gameplay/Characters/PSCharacterBase.h"

#define ECC_Character ECC_GameTraceChannel1

APSTriggerActor::APSTriggerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(RootComponent);

	TriggerComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerComponent->SetCollisionResponseToChannel(ECC_Character, ECR_Overlap);
}


void APSTriggerActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APSTriggerActor::OnBeginOverlap);
}


void APSTriggerActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APSCharacterBase* Character = Cast<APSCharacterBase>(OtherActor))
	{
		//DoAction(Character);
	}
}


void APSTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

