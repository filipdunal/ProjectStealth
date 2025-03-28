// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PSSettingsObject.generated.h"

/**
 *  Base settings object for settings visible in Project Settings
 */
UCLASS(Abstract, config = Game, DefaultConfig)
class PROJECTSTEALTH_API UPSSettingsObject : public UObject
{
	GENERATED_BODY()

public:
	virtual FName GetSettingsName() const PURE_VIRTUAL(UPSSettingsObject::GetSettingsName, return NAME_None;);
};
