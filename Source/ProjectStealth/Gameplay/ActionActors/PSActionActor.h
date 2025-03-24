// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "ProjectStealth/Gameplay/Interfaces/PSTriggerSource.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSActionActor.generated.h"


class APSCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivityChanged, bool, bNewActive);


UCLASS(Abstract)
class PROJECTSTEALTH_API APSActionActor : public AActor
{
	GENERATED_BODY()

public:	
	APSActionActor();

	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	FORCEINLINE bool IsActive() const { return bIsActive; };

	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	void SetActive(bool bNewActive);

	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FActivityChanged OnActivityChanged;

	// Checks if action can be performed and performs it. Returns true if it can be performed.
	UFUNCTION(BlueprintCallable, Category = "Project Stealth")
	bool TryPerformAction(TScriptInterface<IPSTriggerSource> TriggerSource);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Project Stealth")
	void PerformAction(const TScriptInterface<IPSTriggerSource>& TriggerSource);
	//virtual void PerformAction_Implementation(TScriptInterface<IPSTriggerSource> TriggerSource) PURE_VIRTUAL(APSActionActor, );


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Stealth")
	bool bIsActive;

public:
	virtual void Tick(float DeltaTime) override;
};
