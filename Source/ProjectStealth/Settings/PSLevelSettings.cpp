// Copyright (c) 2025 Filip Dunal. All Rights Reserved.


#include "ProjectStealth/Settings/PSLevelSettings.h"


bool UPSLevelSettings::FindLevel(FName LevelName, FPSLevel& FoundLevel)
{
	UPSLevelSettings* CDO = StaticClass()->GetDefaultObject<UPSLevelSettings>();

	const FPSLevel* LevelPtr = CDO->Levels.FindByPredicate([LevelName](const FPSLevel& Level)
		{
			return LevelName == Level.Name;
		});

	FoundLevel = *LevelPtr;
	return LevelPtr != nullptr;
}


TArray<FName> UPSLevelSettings::GetLevelNames()
{
	UPSLevelSettings* CDO = StaticClass()->GetDefaultObject<UPSLevelSettings>();

	TArray<FName> LevelNames;
	for(const FPSLevel& Level: CDO->Levels)
	{
		LevelNames.Add(Level.Name);
	}
	return LevelNames;
}
