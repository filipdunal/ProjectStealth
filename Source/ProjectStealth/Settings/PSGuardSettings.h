// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/Settings/PSSettingsObject.h"
#include "PSGuardSettings.generated.h"


UCLASS(config = Project, DefaultConfig, meta = (DisplayName = "Guard Settings"))
class PROJECTSTEALTH_API UPSGuardSettings : public UPSSettingsObject
{
	GENERATED_BODY()

public:

	virtual FName GetSettingsName() const override { return TEXT("Guard Settings");};


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config)
	TSubclassOf<UUserWidget> GuardWidgetClass;
};
