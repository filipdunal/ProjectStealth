// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectStealth.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Settings/PSSettingsObject.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectStealth, ProjectStealth, "ProjectStealth");


void FProjectStealth::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();

#if WITH_EDITOR
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		for(TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
		{
			if(const UClass* Class = *ClassIt)
			{
				if(Class->IsNative() && !Class->HasAnyClassFlags(CLASS_Abstract) && Class->IsChildOf<UPSSettingsObject>())
				{
					UPSSettingsObject* CDO = Class->GetDefaultObject<UPSSettingsObject>();

					const FName SettingsName = CDO->GetSettingsName();
					const FText SettingsText = FText::FromName(SettingsName);

					SettingsModule->RegisterSettings(
						"Project",
						"Project Stealth",
						SettingsName,
						SettingsText,
						SettingsText,
						CDO
					);
				}
			}
		}
	}
#endif

}


void FProjectStealth::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}
