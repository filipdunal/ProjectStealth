// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/Gameplay/Components/PSTriggerComponent.h"
#include "PSOverlapTriggerComponent.generated.h"


UCLASS(meta = (BlueprintSpawnableComponent))
class PROJECTSTEALTH_API UPSOverlapTriggerComponent : public UPSTriggerComponent
{
	GENERATED_BODY()

protected:

	UPSOverlapTriggerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
