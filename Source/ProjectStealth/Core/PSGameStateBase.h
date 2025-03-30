// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PSGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSGameOver, bool, bWin);

UCLASS()
class PROJECTSTEALTH_API APSGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FPSGameOver OnGameOver;
};
