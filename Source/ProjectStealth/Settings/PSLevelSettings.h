// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/Settings/PSSettingsObject.h"
#include "PSLevelSettings.generated.h"

USTRUCT(BlueprintType)
struct FPSLevel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	FName Name = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	FText ShortDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	TSoftObjectPtr<UWorld> LevelObject;
};


UCLASS(config = Project, DefaultConfig, meta = (DisplayName = "Level Settings"))
class PROJECTSTEALTH_API UPSLevelSettings : public UPSSettingsObject
{
	GENERATED_BODY()
public:

	virtual FName GetSettingsName() const override { return TEXT("Level Settings"); }

	UFUNCTION(BlueprintCallable, Category = "Level Settings")
	static bool FindLevel(FName LevelName,FPSLevel& FoundLevel);

	UFUNCTION(BlueprintCallable, Category = "Level Settings")
	static TArray<FName> GetLevelNames();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, meta=(TitleProperty="{Name}"), Category = "Level Settings")
	TArray<FPSLevel> Levels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	TSoftObjectPtr<UWorld> MainMenuLevel;
};
