// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PSInteractionComponent.generated.h"


class APSActionActor;
class UPSInteractTriggerComponent;
class APSInteractiveActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentTriggerChanged, UPSInteractTriggerComponent*, Previous, UPSInteractTriggerComponent*, Current);

UCLASS()
class PROJECTSTEALTH_API UPSInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpdateCurrentTrigger();


	UPROPERTY()
	TArray<TObjectPtr<UPSInteractTriggerComponent>> OverlappedTriggers;

	UPROPERTY()
	TObjectPtr<UPSInteractTriggerComponent> CurrentTrigger;

	FTimerHandle UpdateCurrentTriggerHandle;

public:

	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FCurrentTriggerChanged OnCurrentTriggerChanged;


	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	FORCEINLINE UPSInteractTriggerComponent* GetCurrentTrigger() const { return CurrentTrigger; };
};
