// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PSTriggerSource.generated.h"


enum class EPSSuspicionLevel : uint8;

UINTERFACE(Blueprintable)
class UPSTriggerSource : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSTEALTH_API IPSTriggerSource
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Project Stealth")
	EPSSuspicionLevel GetSuspicionLevel();
};
