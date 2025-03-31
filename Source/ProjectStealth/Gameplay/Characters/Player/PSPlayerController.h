// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PSPlayerController.generated.h"


class UPSInteractTriggerComponent;
class APSHUD;
class UPSInteractionComponent;

UCLASS()
class PROJECTSTEALTH_API APSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	APSPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnInteractTriggerChanged(UPSInteractTriggerComponent* OldTrigger, UPSInteractTriggerComponent* NewTrigger);


	UPROPERTY(BlueprintReadOnly, Category = "Project Stealth")
	TObjectPtr<APSHUD> PSHUD;
};
