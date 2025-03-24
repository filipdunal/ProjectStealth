// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/Gameplay/ActionActors/PSActionActor.h"
#include "PSTriggerActor.generated.h"

UCLASS()
class PROJECTSTEALTH_API APSTriggerActor : public APSActionActor
{
	GENERATED_BODY()
	
public:
	APSTriggerActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Stealth")
	TObjectPtr<UShapeComponent> TriggerComponent;

public:
	virtual void Tick(float DeltaTime) override;
};
