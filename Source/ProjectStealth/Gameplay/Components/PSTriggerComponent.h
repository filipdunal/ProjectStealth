// Copyright (c) 2025 Filip Dunal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ProjectStealth/ProjectStealth.h"
#include "PSTriggerComponent.generated.h"

class IPSTriggerSource;
class APSActionActor;
class APSCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPSTriggerDelegate, TScriptInterface<IPSTriggerSource>, TriggerSource, UPSTriggerComponent*, TriggerComponent);

USTRUCT(BlueprintType)
struct FPSTriggerConeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Stealth")
	float Angle;
};


UCLASS(Abstract)
class PROJECTSTEALTH_API UPSTriggerComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:

	UPSTriggerComponent(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FPSTriggerDelegate OnTriggerBegin;

	UPROPERTY(BlueprintAssignable, Category = "Project Stealth")
	FPSTriggerDelegate OnTriggerEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Stealth")
	TArray<TSubclassOf<APSCharacterBase>> FilterClass;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Project Stealth")
	APSActionActor* GetActionActorChecked() const;

	const TArray<TScriptInterface<IPSTriggerSource>>& GetTriggerSources() const { return TriggerSources; }

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRegister() override;

	bool CanTrigger(const AActor* OtherActor) const;
	bool CheckClassFilter(UClass* ClassToCheck) const;

	void GenerateCubeMesh();
	void GenerateConeMesh(bool bUpdateOnly);

	UFUNCTION()
	void UpdateConeTrigger();


	UPROPERTY()
	TArray<TScriptInterface<IPSTriggerSource>> TriggerSources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Stealth")
	EPSTriggerType TriggerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta= (EditCondition= "TriggerType == EPSTriggerType::Cone"), Category = "Project Stealth")
	FPSTriggerConeSettings TriggerConeSettings;

	FTimerHandle UpdateConeTriggerHandle;

private:

	UFUNCTION()
	void RegisterTriggerSource(TScriptInterface<IPSTriggerSource> TriggerSource, UPSTriggerComponent* TriggerComponent);

	UFUNCTION()
	void UnregisterTriggerSource(TScriptInterface<IPSTriggerSource> TriggerSource, UPSTriggerComponent* TriggerComponent);
};
