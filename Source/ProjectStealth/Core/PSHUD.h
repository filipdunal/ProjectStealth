// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PSHUD.generated.h"


class UPSInteractTriggerComponent;

UCLASS()
class PROJECTSTEALTH_API APSHUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Project Stealth")
	void OnInteractTriggerChanged(UPSInteractTriggerComponent* OldTrigger, UPSInteractTriggerComponent* NewTrigger);
};
