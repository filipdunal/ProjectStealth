// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSGuardWidget.generated.h"

class UPSGuardComponent;

UCLASS()
class PROJECTSTEALTH_API UPSGuardWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, Category = "Project Stealth")
	TWeakObjectPtr<UPSGuardComponent> GuardComponent;
};
