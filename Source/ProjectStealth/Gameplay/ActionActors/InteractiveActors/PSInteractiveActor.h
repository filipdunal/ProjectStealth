// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStealth/Gameplay/ActionActors/PSActionActor.h"
#include "PSInteractiveActor.generated.h"

UCLASS()
class PROJECTSTEALTH_API APSInteractiveActor : public APSActionActor
{
	GENERATED_BODY()
	
public:	
	APSInteractiveActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
