// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EPSTriggerType: uint8
{
	Cone UMETA(DisplayName = "Cone")
};


UENUM()
enum class EPSGuardState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Suspicious UMETA(DisplayName = "Suspicious"),
	Alerted UMETA(DisplayName = "Alerted")
};


UENUM()
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

