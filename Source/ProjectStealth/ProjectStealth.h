// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPSTriggerType: uint8
{
	Cone UMETA(DisplayName = "Cone"),
	Box UMETA(DisplayName = "Box")
};


UENUM(BlueprintType)
enum class EPSGuardState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Suspicious UMETA(DisplayName = "Suspicious"),
	Alerted UMETA(DisplayName = "Alerted")
};


UENUM(BlueprintType)
enum class EPSSuspicionLevel : uint8
{
	None UMETA(DisplayName = "None"),
	Low UMETA(DisplayName = "Low"),
	High UMETA(DisplayName = "High"),
};


class FProjectStealth: public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

