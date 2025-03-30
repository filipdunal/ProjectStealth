// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/Settings/PSSettingsObject.h"
#include "PSTriggerSettings.generated.h"


UCLASS(config = Project, DefaultConfig, meta = (DisplayName = "Trigger Settings"))
class PROJECTSTEALTH_API UPSTriggerSettings : public UPSSettingsObject
{
	GENERATED_BODY()

public:

	virtual FName GetSettingsName() const override { return TEXT("Trigger Settings"); }


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Trigger Settings")
	TSoftObjectPtr<UMaterialInterface> DefaultMaterial;
};
